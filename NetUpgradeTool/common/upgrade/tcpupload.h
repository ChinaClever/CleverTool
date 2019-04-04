#ifndef TCPUPLOAD_H
#define TCPUPLOAD_H

#include "tftpupgrade.h"
#include "tcp/client/tcpclient.h"

typedef struct
{
    QString ip;
    QString file; // 升级文件
    QString usr; // 用户名
    QString pwd; // 密码
}sTcpUpload;

class TcpUpload : public QObject
{
    Q_OBJECT
public:
    explicit TcpUpload(QObject *parent = nullptr);

    void upload(sTcpUpload &tcpStr);
    void breakDown();

protected:
    bool connectServer(const QString &ip);
    bool readFile(const QString &fn);
    bool sentLen(void);
    bool startSent(void);

    bool recvVerify(void);
    bool sentData(void);
    void stepMode(void);
    void progress(void);

signals:
    void connectSig(int);
    void progressSig(float,QString);

protected slots:
    void connectSlot(int);

private:
    TcpClient *mTcpClient;
    sTcpUpload mTcpUpdateStr;
    QByteArray mByFile;
    bool isRun, isStart, isVeried;
    int mSentLen;
};

#endif // TCPUPGRADE_H
