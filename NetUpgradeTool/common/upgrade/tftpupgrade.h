#ifndef TFTPUPGRADE_H
#define TFTPUPGRADE_H

#include "tftp/tftp.h"
#include "upgradethread.h"

class TftpUpgrade : public UpgradeThread
{
    Q_OBJECT
public:
    explicit TftpUpgrade(QObject *parent = nullptr);
    ~TftpUpgrade();

protected:
    bool upload(const QString &file, const QString &ip);
    void breakSent();

private:
    Tftp *mTftp;
};

#endif // TFTPUPGRADE_H
