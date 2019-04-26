/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "configbase.h"

TestConfig::TestConfig()
{
    item = new sTestConfigItem();
}

void TestConfig::initConfig(sSnItem *it)
{
    if(!it) it = &(item->sn);

    it->op = getOp();
    it->cn = getCn();
    it->barCode = getBarCode();
    it->batch = getBatch();
    it->purpose = getPurpose();
    it->snClear = getSnClear();
}

void TestConfig::saveConfig(sSnItem *it)
{
    if(!it) it = &(item->sn);

    setOp(it->op);
    setCn(it->cn);
    setBarCode(it->barCode);
    setBatch(it->batch);
    setPurpose(it->purpose);
    setSnClear(it->snClear);
}

QString TestConfig::getOp()
{
    QString prefix = getPrefix();
    QString str = QString("%1_op").arg(prefix);
    return sys_configFile_readStr(str, prefix);
}

void TestConfig::setOp(const QString &arg)
{
    QString prefix = getPrefix();
    QString str = QString("%1_op").arg(prefix);
    sys_configFile_writeParam(str, arg, prefix);
}


QString TestConfig::getCn()
{
    QString prefix = getPrefix();
    QString str = QString("%1_cn").arg(prefix);
    return sys_configFile_readStr(str, prefix);
}

void TestConfig::setCn(const QString &arg)
{
    QString prefix = getPrefix();
    QString str = QString("%1_cn").arg(prefix);
    sys_configFile_writeParam(str, arg, prefix);
}


QString TestConfig::getBarCode()
{
    QString prefix = getPrefix();
    QString str = QString("%1_barcode").arg(prefix);
    return sys_configFile_readStr(str, prefix);
}

void TestConfig::setBarCode(const QString &arg)
{
    QString prefix = getPrefix();
    QString str = QString("%1_barcode").arg(prefix);
    sys_configFile_writeParam(str, arg, prefix);
}


QString TestConfig::getBatch()
{
    QString prefix = getPrefix();
    QString str = QString("%1_batch").arg(prefix);
    return sys_configFile_readStr(str, prefix);
}

void TestConfig::setBatch(const QString &arg)
{
    QString prefix = getPrefix();
    QString str = QString("%1_batch").arg(prefix);
    sys_configFile_writeParam(str, arg, prefix);
}

QString TestConfig::getPurpose()
{
    QString prefix = getPrefix();
    QString str = QString("%1_purpose").arg(prefix);
    return sys_configFile_readStr(str, prefix);
}

void TestConfig::setPurpose(const QString &arg)
{
    QString prefix = getPrefix();
    QString str = QString("%1_purpose").arg(prefix);
    sys_configFile_writeParam(str, arg, prefix);
}


/**
 * @brief 获取相数
 * @return
 */
bool TestConfig::getSnClear()
{
    bool ret = true;
    QString prefix = getPrefix();
    QString str = QString("%1_snclear").arg(prefix);
    int rtn = sys_configFile_readInt(str, prefix);
    if(rtn == 0)  ret = false;
    return ret;
}

/**
 * @brief 设置相数
 * @param num
 */
void TestConfig::setSnClear(bool mode)
{
    int ret = 0;
    if(mode) ret = 1;
    QString prefix = getPrefix();
    QString str = QString("%1_snclear").arg(prefix);
    sys_configFile_writeParam(str, QString::number(ret), prefix);
}
