#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtCore>
#include <QtNetwork>

typedef enum {
    UP_CMD_CONNECT, // 连接成功
    UP_CMD_READ, // 读取数据
    UP_CMD_SENT_OK, // 发送完成
    UP_CMD_DIS, //  断开连接
    UP_CMD_ERR, // 连接错误
    UP_CMD_VERIFY,// 验证成功
    UP_CMD_PWDERR, // 账号密码错误
}UP_TCP_CMD;
#define TCP_PORT	30966 // TCP端口号

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    ~TcpClient();

    bool newConnect(const QString &host, int port=TCP_PORT); //连接服务器
    bool sentMessage(uchar *buf,  int len);
    bool sentMessage(const QByteArray &data);
    bool getSentStatue(void);
    void closeConnect(void);
    int getData(QByteArray &data);

protected:
    int writeMessage(char *buf, int len);
    int writeMessage(QByteArray &data);

signals:
    void connectSig(int);
    void disconnectedSig(); // 断开信号
    void sentErr(QString);

protected slots:
    void timeoutDone(void);
    void connectedSlot();
    void disconnectedSlot();
    void readMessageSlot();  //接收数据
    void displayError(QAbstractSocket::SocketError);  //显示错误

private:
    QHostAddress *mServerIP;
    QTcpSocket *mTcpSocket;
    QList<QByteArray> mSentData;
    QByteArray mRecvData;
};

bool get_tcp_connect(void);

#endif // TCPCLIENT_H
