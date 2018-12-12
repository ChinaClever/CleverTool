#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>

/**
 * 数据包
 */
struct sDataPacket
{
    bool isRun;
    int progress;

    QString file;
    QString status;

    QStringList ips;
    QStringList errs;
    QStringList oks;
    QList<QStringList> logs;
};


class DataPacket
{
    DataPacket();
public:
    static DataPacket *bulid();

    sDataPacket *data;
};

#endif // DATAPACKET_H
