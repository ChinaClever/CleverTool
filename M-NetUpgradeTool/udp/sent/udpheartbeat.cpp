/*
 * udpheartbeat.cpp
 * UDP 心跳包线程
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpheartbeat.h"

#define UDP_HEARTBEAT_MSG   "MPDU Net Upgrade Tool Server OK!" // 心跳包

UdpHeartBeat::UdpHeartBeat(QObject *parent) : QThread(parent)
{
    isRun=false;
    mSocket = NULL;
    startSent();
}


UdpHeartBeat::~UdpHeartBeat()
{
    isRun=false;
    wait();
}

UdpHeartBeat *UdpHeartBeat::bulid(QObject *parent)
{
    static UdpHeartBeat* sington = NULL;
    if(sington == NULL) {
        sington = new UdpHeartBeat(parent);
    }
    return sington;
}

/**
 * @brief 启动心跳包线程
 */
void UdpHeartBeat::startSent(void)
{
    mSocket = new UdpSentSocket(this);

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1500);

    mUdpBDSent = new UdpBDSent(this); // 启动UDP广播数据包
}

void UdpHeartBeat::timeoutDone(void)
{
//    start(); // 在定时器中启动线程
    run();
}

/**
* @brief 心跳包
*/
void UdpHeartBeat::heartbeatPacket(void)
{
    QByteArray msg = UDP_HEARTBEAT_MSG;

    if(mSocket)
        mSocket->sentBroadcastData(msg,UDP_HB_PORT);
}


void UdpHeartBeat::run(void)
{
    if(isRun != true) // 在运行就不执行
    {
        isRun = true;
        heartbeatPacket();
        isRun = false;
    }
}

