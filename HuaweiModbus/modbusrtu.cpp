#include "modbusrtu.h"

ModbusRtu::ModbusRtu(QObject *parent) : QObject(parent)
{

}


bool ModbusRtu::sentCmd(int reg, int len)
{
    QByteArray array;

    01 03 01 00 00 14 44 39

}
