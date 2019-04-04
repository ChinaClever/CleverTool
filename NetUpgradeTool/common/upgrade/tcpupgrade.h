#ifndef TCPUPGRADE_H
#define TCPUPGRADE_H

#include "tcpupload.h"

class TcpUpgrade : public UpgradeThread
{
    Q_OBJECT
public:
    explicit TcpUpgrade(QObject *parent = nullptr);
    ~TcpUpgrade();

protected:
    bool upload(const QString &file, const QString &ip);
    void breakSent();

public slots:
    void connectSlot(int);

private:
    int mStatus;
    QString mIP;
    TcpUpload *mTcpSent;
};

#endif // TCPUPGRADE_H
