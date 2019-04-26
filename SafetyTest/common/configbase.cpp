/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "configbase.h"

ConfigBase::ConfigBase()
{
    item = new sConfigItem();
    item->setMode = Test_SNMP;
    item->devNum = 1;
    item->lineNum = 1;
    item->cmdModel = 1;
    item->msecs = 10;
    item->outputNum = 24;
    item->serial = nullptr;
    item->v = 0;
}


/**
 * @brief 获取串口名称
 * @return 串口名
 */
QString ConfigBase::getSerialName()
{
    QString prefix = getPrefix();
    QString str = QString("%1_COM").arg(prefix);
    return sys_configFile_readStr(str, prefix);
}

/**
 * @brief 设置串口名
 * @param name
 */
void ConfigBase::setSerialName(const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_COM").arg(prefix);
    sys_configFile_writeParam(str, name, prefix);
}


/**
 * @brief 获取相数
 * @return
 */
int ConfigBase::getTestMode()
{
    QString prefix = getPrefix();
    QString str = QString("%1_test_mode").arg(prefix);
    int ret = sys_configFile_readInt(str, prefix);
    if(ret < 0)  ret = 0;
    return ret;
}

/**
 * @brief 设置相数
 * @param num
 */
void ConfigBase::setTestMode(int mode)
{
    QString prefix = getPrefix();
    QString str = QString("%1_test_mode").arg(prefix);
    sys_configFile_writeParam(str, QString::number(mode), prefix);
}


/**
 * @brief 获取串口名称
 * @return 串口名
 */
QString ConfigBase::getIp()
{
    QString prefix = getPrefix();
    QString str = QString("%1_ip").arg(prefix);
    return sys_configFile_readStr(str, prefix);
}

/**
 * @brief 设置串口名
 * @param name
 */
void ConfigBase::setIp(const QString &ip)
{
    QString prefix = getPrefix();
    QString str = QString("%1_ip").arg(prefix);
    sys_configFile_writeParam(str, ip, prefix);
}

/**
 * @brief 获取相数
 * @return
 */
int ConfigBase::getLineNum()
{
    QString prefix = getPrefix();
    QString str = QString("%1_line_num").arg(prefix);
    int ret = sys_configFile_readInt(str, prefix);
    if(ret <= 0)  ret = 1;
    return ret;
}

/**
 * @brief 设置相数
 * @param num
 */
void ConfigBase::setLineNum(int num)
{
    QString prefix = getPrefix();
    QString str = QString("%1_line_num").arg(prefix);
    sys_configFile_writeParam(str, QString::number(num), prefix);
}


/**
 * @brief 获取级联数量
 * @return
 */
int ConfigBase::getDevNum()
{
    QString prefix = getPrefix();
    QString str = QString("%1_dev_num").arg(prefix);
    int ret = sys_configFile_readInt(str, prefix);
    if(ret <= 0)  ret = 1;

    return ret;
}

/**
 * @brief 设置级联数据
 * @param num
 */
void ConfigBase::setDevNum(int num)
{
    QString prefix = getPrefix();
    QString str = QString("%1_dev_num").arg(prefix);
    sys_configFile_writeParam(str, QString::number(num), prefix);
}

/**
 * @brief 获取Modbus命令模式的模式
 * @return
 */
int ConfigBase::getModbusCmd()
{
    QString prefix = getPrefix();
    QString str = QString("%1_modbus_cmd").arg(prefix);
    int ret = sys_configFile_readInt(str, prefix);
    if(ret <= 0)  ret = 1;
    return ret;
}

/**
 * @brief 设置Modbus命令模
 * @param num
 */
void ConfigBase::setModbusCmd(int num)
{
    QString prefix = getPrefix();
    QString str = QString("%1_modbus_cmd").arg(prefix);
    sys_configFile_writeParam(str, QString::number(num), prefix);
}

/**
 * @brief 获取Modbus 传输时间间隔Time
 * @return
 */
int ConfigBase::getModbusTime()
{
    QString prefix = getPrefix();
    QString str = QString("%1_modbus_time").arg(prefix);
    int ret = sys_configFile_readInt(str, prefix);
    if(ret <= 0)  ret = 1;
    return ret;
}

/**
 * @brief 设置Modbus 传输时间间隔Time
 * @return
 */
void ConfigBase::setModbusTime(int num)
{
    QString prefix = getPrefix();
    QString str = QString("%1_modbus_time").arg(prefix);
    sys_configFile_writeParam(str, QString::number(num), prefix);
}


/**
 * @brief 获取日志存储时间隔
 * @return
 */
int ConfigBase::getLogTime()
{
    QString prefix = getPrefix();
    QString str = QString("%1_log_time").arg(prefix);
    int ret = sys_configFile_readInt(str, prefix);
    if(ret <= 0)  ret = 60;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setLogTime(int num)
{
    QString prefix = getPrefix();
    QString str = QString("%1_log_time").arg(prefix);
    sys_configFile_writeParam(str, QString::number(num), prefix);
}


int ConfigBase::getOutputNum()
{
    QString prefix = getPrefix();
    QString str = QString("%1_output_num").arg(prefix);
    int ret = sys_configFile_readInt(str, prefix);
    if(ret <= 0)  ret = 24;
    return ret;
}

/**
 * @brief 设置相数
 * @param num
 */
void ConfigBase::setOutputNum(int num)
{
    QString prefix = getPrefix();
    QString str = QString("%1_output_num").arg(prefix);
    sys_configFile_writeParam(str, QString::number(num), prefix);
}


int ConfigBase::getVersion()
{
    QString prefix = getPrefix();
    QString str = QString("%1_version_num").arg(prefix);
    int ret = sys_configFile_readInt(str, prefix);
    if(ret <= 0)  ret = 0;
    return ret;
}

/**
 * @brief 设置相数
 * @param num
 */
void ConfigBase::setVersion(int num)
{
    QString prefix = getPrefix();
    QString str = QString("%1_version_num").arg(prefix);
    sys_configFile_writeParam(str, QString::number(num), prefix);
}

/**
 * @brief 获取电阻值
 * @param addr bit
 */
int ConfigBase::getResistance(int addr,int bit)
{
    QString prefix = getPrefix();
    QString str = QString("Res%1%2").arg(addr).arg(bit);
    int ret = sys_configFile_readInt(str, prefix);
    if(ret <= 0)  ret = 0;
    return ret;
}

/**
 * @brief 设置相数
 * @param num
 */
void ConfigBase::setResistance(int addr,int bit,int num)
{
    QString prefix = getPrefix();
    QString str = QString("Res%1%2").arg(addr).arg(bit);
    sys_configFile_writeParam(str, QString::number(num), prefix);
}
