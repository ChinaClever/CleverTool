#ifndef MACHINECODE_H
#define MACHINECODE_H
#include "registrykey.h"

class MachineCode
{
public:
    MachineCode();

    QString getMachineCode(int type=0); // 获取机器码

protected:
    QString getWMIHWInfo(int type);
    QString getCPUID1();
    QString getCPUID2();
    QString getHDLogicalID();
    QString getMac();
    QString getCPUManID();
};

#endif // MACHINECODE_H
