#include "udptesting.h"

UdpTesting::UdpTesting(QObject *parent) : QObject(parent)
{
    UdpHeartBeat::bulid(this);
    mRecv = new UdpRecvSocket(this);
    mRecv->initSocket(UDP_HB_PORT);

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(2500);
}

bool UdpTesting::check(QStringList &ips)
{
    bool ret = false;
    ips = mIPs;
    if(ips.size() > 1) ret = true;

    return ret;
}

void UdpTesting::timeoutDone(void)
{
    QList<UdpBaseData *> list;
    UdpBaseData *data = nullptr;
    do {
         data = mRecv->getData();
         if(data) list.append(data);
    } while (data);

    QStringList ips;
    for(int i=0; i<list.size(); ++i) {
        UdpBaseData *data = list.at(i);
        QString ip =  data->addr.toString();
        if(!ips.contains(ip)) ips << ip;
        delete data;
    }
    mIPs = ips;
}
