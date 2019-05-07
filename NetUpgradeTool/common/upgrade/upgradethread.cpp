#include "upgradethread.h"

UpgradeThread::UpgradeThread(QObject *parent) : QThread(parent)
{
    mData = DataPacket::bulid()->data;
}


UpgradeThread::~UpgradeThread()
{
//    breakDown();
//    wait();
}

/**
 * @brief 设置需要发送的IP，和文件
 * @param ip
 * @param file
 */
bool UpgradeThread::startSend()
{
    if(mData->isRun) return false;
    start();

    return true;
}

void UpgradeThread::subProgressSlot(float f, QString s)
{
    mData->subPorgress = (int)f;
    mData->subStatus = s;
}

void UpgradeThread::saveLogs(const QString &ip, bool f, bool exist)
{
    QStringList str;

    str << QString::number(mData->progress);
    str << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    str << ip;
    if(exist){
        if(f) str << tr("成功");
            //str << tr("successful");
        else str << tr("失败");
            //str << tr("failure");
    } else {
        str << tr("IP不存在");
        //str << tr("IP isn't existence");
    }

    mData->logs << str;
}

/**
 * @brief 发送文件
 * @return
 */
bool UpgradeThread::sentFile(void)
{
    QString ip = mIps.first();
    mIps.removeFirst();

    bool exist = checkIp(ip);
    bool ret = false;
    if(exist) {
        //mData->status = tr("starting transmission：%1").arg(ip);
        mData->status = tr("开始传输：%1").arg(ip);
        ret = upload(mData->file, ip);
        if(ret){
            mData->oks << ip;
            mData->status = tr("传输成功：%1").arg(ip);
            //mData->status = tr("transmission successful:%1").arg(ip);
        } else {
            mData->errs << ip;
            mData->status = tr("传输失败：%1").arg(ip);
            //mData->status = tr("transmission failure：%1").arg(ip);
        }
    }  else {
        mData->errs << ip;
        mData->status = tr("目标IP不存在：%1").arg(ip);
         //mData->status = tr("destination IP isn't existence：%1").arg(ip);
    }

    mData->progress++;
    saveLogs(ip, ret , exist);

    return ret && exist;
}

void UpgradeThread::initSend()
{
    mIps = mData->ips;
    mData->isRun = true;

    mData->progress = 0;
    mData->status.clear();
    mData->errs.clear();
    mData->oks.clear();
    mData->logs.clear();
}

void UpgradeThread::overSend()
{
    mData->isRun = false;

    QString str;
    if(mData->errs.size()) {
        str = tr("%1台失败 %2台成功 ").arg(mData->errs.size()).arg(mData->oks.size());
        //str = tr("%1 failure %2 successful").arg(mData->errs.size()).arg(mData->oks.size());
    } else {
        str = tr("%1台成功 ").arg(mData->oks.size());
        //str = tr("%1 successful ").arg(mData->oks.size());
    }
    mData->status = str;
}

void UpgradeThread::run(void)
{
    initSend();

    while (mIps.size() > 0) {
        sentFile();
        if(mIps.size()) sleep(4);
    }
    overSend();
}

void UpgradeThread::breakDown()
{
    mIps.clear();
    breakSent();
    wait();
}

bool UpgradeThread::checkIp(const QString& ip)
{
    QProcess pingProcess;
    QString strArg = "ping " + ip + " -n 1 -i 2";  //strPingIP 为设备IP地址
    pingProcess.start(strArg,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = QString::fromLocal8Bit( pingProcess.readAllStandardOutput());
    bool bPingSuccess = false;

    if(p_stdout.contains("TTL=")) //我采用这个字符来判断 对不对？
    {
        bPingSuccess = true;
    }else
    {
        bPingSuccess = false;
    }
    return bPingSuccess;
}
