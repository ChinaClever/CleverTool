#ifndef TFTPTHREAD_H
#define TFTPTHREAD_H

#include <QThread>
#include "tftp.h"

class TftpThread : public QThread
{
    Q_OBJECT
public:
    explicit TftpThread(QObject *parent = 0);
    ~TftpThread();

    bool startSend();

protected:
    void run(void);
    bool sentFile(void);

    void saveLogs(const QString &ip, bool f , bool exist);
    void initSend();
    void overSend();

protected slots:
    void subProgressSlot(float f, QString);

private:
    bool checkIp(const QString &ip);
    Tftp *mTftp;
    sDataPacket *mData;
    QStringList mIps;
};

#endif // TFTPTHREAD_H
