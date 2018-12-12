#include "qsshmanager.h"

QSshManager::QSshManager(QObject *parent) : QObject(parent),  m_session(nullptr), m_socket(INVALID_SOCKET)
{
    libssh2_init(0);
}

QSshManager::~QSshManager()
{
    if (!m_channels.empty()){
        for (auto channel : m_channels) if (nullptr != channel) libssh2_channel_free(channel);
        m_channels.clear();
    }
    if (!m_channel_mutexs.empty()){
        for (auto mtx : m_channel_mutexs) delete mtx;
        m_channel_mutexs.clear();
    }

    if (nullptr != m_session){
        libssh2_session_disconnect(m_session, "Bye bye, Thank you");
        libssh2_session_free(m_session);
        m_session = nullptr;
    }

    libssh2_exit();

    if (INVALID_SOCKET != m_socket) closesocket(m_socket);

#ifdef WIN32
    WSACleanup();
#endif
}

bool QSshManager::connect(const char *szIp, int nPort)
{

#ifdef WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return false;
#endif

    m_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(nPort);
    sin.sin_addr.s_addr = inet_addr(szIp);
    return ::connect(m_socket, (sockaddr*)(&sin), sizeof(sockaddr_in)) == 0;
}

bool QSshManager::login(const char *szUserName, const char *szPassword)
{
    m_session = libssh2_session_init();

    if (libssh2_session_handshake(m_session, m_socket)) return false;

    libssh2_hostkey_hash(m_session, LIBSSH2_HOSTKEY_HASH_SHA1);

    libssh2_userauth_list(m_session, szUserName, strlen(szUserName));

    return !libssh2_userauth_password(m_session, szUserName, szPassword);
}


int QSshManager::createChannel(const char *szChannelTerm)
{
    if (nullptr == m_session) return -1;

    LIBSSH2_CHANNEL* channel = libssh2_channel_open_session(m_session);

    if (channel && !libssh2_channel_request_pty(channel, szChannelTerm))
    {
        if (!libssh2_channel_shell(channel)) {
            m_channels.push_back(channel);
            m_channel_mutexs.push_back(new std::mutex);
            return m_channels.size() - 1;
        }
    }

    if (channel) libssh2_channel_free(channel);

    return -1;
}

std::string QSshManager::read(int channelNo, const char *szEnd, int timeout)
{
    std::string data;

    if (channelNo > (m_channels.size() - 1)) return data;

    std::lock_guard<std::mutex> locker(*m_channel_mutexs.at(channelNo));

    LIBSSH2_CHANNEL * channel = m_channels.at(channelNo);

    if (nullptr == channel) return data;

    std::shared_ptr<LIBSSH2_POLLFD> fds = std::make_shared<LIBSSH2_POLLFD>();

    fds->type = LIBSSH2_POLLFD_CHANNEL;
    fds->fd.channel = channel;
    fds->events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;

    auto compareEndNoSpace = [](const std::string& str, const std::string& endstr){
        size_t pos = str.rfind(endstr);
        return pos != std::string::npos && str.substr(pos, endstr.size()) == endstr;
    };

    do{
        int rc = (libssh2_poll(fds.get(), 1, 10));

        if ((rc >= 1) && (fds->revents & LIBSSH2_POLLFD_POLLIN))
        {
            char buffer[64 * 1024] = {0};
            size_t n = libssh2_channel_read(channel, buffer, sizeof(buffer));

            if (n != LIBSSH2_ERROR_EAGAIN){
                if (n > 0) data.append(buffer);
                if (n <= 0 || strlen(szEnd) == 0 || compareEndNoSpace(data,szEnd)) return data;
            }
        }

        timeout -= 50;
        Sleep(50);
    }while (timeout > 0);

    return data;
}

bool QSshManager::write(int channelNo, const char *szData)
{
    std::lock_guard<std::mutex> locker(*m_channel_mutexs.at(channelNo));

    LIBSSH2_CHANNEL * channel = m_channels.at(channelNo);

    if (nullptr == channel) return false;

    std::string send_data = szData;
    send_data.append("\n");

    return libssh2_channel_write_ex(channel, 0, send_data.c_str(), send_data.size()) == send_data.size();
}
