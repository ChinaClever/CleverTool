#include "websocketclient.h"
#include "sysconfigfile.h"

WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent)
{
    isConnected = false;
    mSocket = new QWebSocket();

    connect(mSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(textMessageReceived(QString)));
}

void WebSocketClient::open(const QUrl &url)
{
    if(!url.isEmpty()) {
        mSocket->open(url);
        m_url = url;
    }
}

void WebSocketClient::open()
{
    readFile();
    open(m_url);
}

void WebSocketClient::readFile()
{
    QString url = sys_configFile_readStr("url", WEB_SOCKET);
    if(url.isEmpty()) {
        url = "localhost";
        sys_configFile_writeParam("url", "localhost", WEB_SOCKET);
    }

    QString port = sys_configFile_readStr("port", WEB_SOCKET);
    if(port.isEmpty()) {
        port = "2346";
        sys_configFile_writeParam("port", "2346", WEB_SOCKET);
    }
    m_url = "ws://" + url +":" + port;
}

bool WebSocketClient::sendMessage(const QJsonObject &message)
{
    bool ret = false;
    if(isConnected) {
        QJsonDocument doc(message);
        QString str = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
        int rtn = mSocket->sendTextMessage(str);
        if(rtn > 0)  ret = true;
        mSocket->flush();
    } else {
        open(m_url);
    }

    return ret;
}



void WebSocketClient::connected()
{
    isConnected = true;
}

void WebSocketClient::disconnected()
{
    isConnected = false;
    emit closed();
}

void WebSocketClient::textMessageReceived(const QString &message)
{
    mRecvList.append(message);
    //    qDebug() << "WebSocketClient recv" <<message;
}

QString WebSocketClient::getMessage()
{
    QString str = mRecvList.first();
    if(!str.isEmpty()) {
        mRecvList.removeFirst();
    }

    return str;
}
