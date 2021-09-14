#ifndef MODBUSTRU_H
#define MODBUSTRU_H

#include "serialport.h"

struct sRtuItem {
    sRtuItem():fn(3),reg(0),num(0){} // 下位机有问题
    uchar addr; // 表示从机地址码
    uchar fn;  // 表示功能码
    ushort reg; // 表示寄存器首地址
    ushort num; // 表示数据长度
    ushort crc; // 表示CRC校验
};


class ModbusRtu : public QObject
{
    Q_OBJECT
public:
    explicit ModbusRtu(QObject *parent = nullptr);

    bool sentCmd(int reg, int len);

signals:

};

#endif // MODBUSTRU_H
