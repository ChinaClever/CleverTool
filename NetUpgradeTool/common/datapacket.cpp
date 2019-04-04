#include "datapacket.h"

DataPacket::DataPacket()
{
    data = new sDataPacket;
    data->isRun = false;
    data->devtype = 0;
    data->progress = 0;
    data->subPorgress = 0;
}

DataPacket *DataPacket::bulid()
{
    static DataPacket* sington = nullptr;
    if(sington == nullptr)
        sington = new DataPacket();
    return sington;
}
