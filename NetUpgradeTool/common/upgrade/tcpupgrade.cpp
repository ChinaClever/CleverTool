#include "tcpupgrade.h"
#include "msgbox.h"

TcpUpgrade::TcpUpgrade(QObject *parent) : UpgradeThread(parent)
{
    mTcpSent = new TcpUpload(this);
    connect(mTcpSent, SIGNAL(connectSig(int)), this, SLOT(connectSlot(int)));
    connect(mTcpSent, SIGNAL(progressSig(float,QString)), this, SLOT(subProgressSlot(float,QString)));
}

TcpUpgrade::~TcpUpgrade()
{
    breakSent();
    wait();
}

bool TcpUpgrade::upload(const QString &file, const QString &ip)
{
    sDataPacket *data = DataPacket::bulid()->data;

    sTcpUpload tcpStr;
    tcpStr.ip = ip;
    tcpStr.file = file;
    tcpStr.usr = data->usr;
    tcpStr.pwd = data->pwd;

    mIP = ip;
    mStatus = 0;
    mTcpSent->upload(tcpStr);

    for(int i=0; i<10*60; ++i) {
        if(mStatus) break;
        else sleep(1);        
    }

    bool ret = false;
    if(mStatus > 0) ret = true;
     breakSent();

    return ret;
}

void TcpUpgrade::breakSent()
{
    mStatus = -1;
    mTcpSent->breakDown();
}


void TcpUpgrade::connectSlot(int step)
{
    QString msg;
    switch (step) {
    case UP_CMD_SENT_OK:
        mStatus = 1;
        break;

    case UP_CMD_PWDERR:
        msg = tr(" 用户名错误，传输失败!!");
        // msg = tr(" username error，transmission failure!!");
        break;

    case UP_CMD_DIS: // 只有当断开、链接错误时，才发出相应信号出来
    case UP_CMD_ERR:
         msg = tr(" 传输失败，请检查网络!!");
        //msg = tr(" transmission failure，please check network!!");
        break;
    }

    if(!msg.isEmpty()) {
        mStatus = -1;
        CriticalMsgBox box(0, mIP+msg);
    }
}
