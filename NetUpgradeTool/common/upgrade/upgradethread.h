#ifndef UPGRADETHREAD_H
#define UPGRADETHREAD_H

#include <QThread>
#include "datapacket.h"

class UpgradeThread : public QThread
{
    Q_OBJECT
public:
    explicit UpgradeThread(QObject *parent = nullptr);
    ~UpgradeThread();

    bool startSend();
    void breakDown();

protected:
    void run(void);
    bool sentFile(void);

    void saveLogs(const QString &ip, bool f , bool exist);
    void initSend();
    void overSend();

    virtual bool upload(const QString &file, const QString &ip) = 0;
    virtual void breakSent()=0;

protected slots:
    void subProgressSlot(float f, QString);

protected:
    bool checkIp(const QString &ip);
    sDataPacket *mData;
    QStringList mIps;
};

#endif // UPGRADETHREAD_H
