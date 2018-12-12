#ifndef QSSHMANAGER_H
#define QSSHMANAGER_H

#include <QObject>
#include <QtCore>
#include <libssh2.h>
#include <vector>
#include <mutex>
#include <memory>
//#include <vld.h>

#ifdef WIN32
#pragma comment(lib,"Ws2_32.lib")
#endif

class QSshManager : public QObject
{
    Q_OBJECT
public:
    explicit QSshManager(QObject *parent = nullptr);
    virtual ~QSshManager();

    bool connect(const char *szIp, int nPort = 22);
    bool login(const char *szUserName, const char *szPassword);

    // 返回值表示渠道的序号。如果返回-1，创建失败；
    int createChannel(const char *szChannelTerm = "vanilla");

    std::string read(int channelNo, const char *szEnd = "$", int timeout = 3000);
    bool write(int channelNo, const char *szData);

private:
    SOCKET m_socket;
    LIBSSH2_SESSION *m_session;
    std::vector<LIBSSH2_CHANNEL*> m_channels;
    std::vector<std::mutex *> m_channel_mutexs;
};

#endif // QSSHMANAGER_H
