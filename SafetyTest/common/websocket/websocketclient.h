#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QtCore>
#include <QWebSocket>

#define WEB_SOCKET  "websocket"

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(QObject *parent = nullptr);

    void open();
    void open(const QUrl &url);
    bool sendMessage(const QJsonObject &message);
    QString getMessage();

signals:
    void closed();

protected:
    void readFile();

protected slots:
    void connected();
    void disconnected();
    void textMessageReceived(const QString &message);

private:
    QWebSocket *mSocket;
    bool isConnected;
    QUrl m_url;
    QStringList mSentList, mRecvList;
    QTimer *timer;
};

#endif // WEBSOCKETCLIENT_H
