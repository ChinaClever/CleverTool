#include "tftpupgrade.h"

TftpUpgrade::TftpUpgrade(QObject *parent) : UpgradeThread(parent)
{
    mTftp = new Tftp(this);
    connect(mTftp, SIGNAL(progressSig(float,QString)), this, SLOT(subProgressSlot(float,QString)));
}

TftpUpgrade::~TftpUpgrade()
{
    breakSent();
    wait();
}

bool TftpUpgrade::upload(const QString &file, const QString &ip)
{
    return mTftp->upload(file, ip);
}

void TftpUpgrade::breakSent()
{
    mTftp->breakDown();
}
