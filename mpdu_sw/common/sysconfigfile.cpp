/*
 * sysconfigfile.cpp
 *  配置文件的操作接口
 *  配置参数的保存与读取
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sysconfigfile.h"
#include <QtCore>

extern QString cm_pathOfData(const QString& name);
static QSettings *pConfigIni = NULL;

/**
 * 功 能：打开系统配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
bool sys_configFile_open(void)
{
    QString strFilename = cm_pathOfData( "sysconfig.ini");
    bool ret = QFileInfo(strFilename).exists();
    if(pConfigIni==NULL) {
        pConfigIni = new QSettings(strFilename, QSettings::IniFormat);
        //        pConfigIni->setIniCodec(QTextCodec::codecForName("utf-8")); // gb18030
    }

    return ret;
}

/**
 * 功 能：关闭系统配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
void sys_configFile_close(void)
{
    // delete pConfigIni;
    // pConfigIni = NULL;

    pConfigIni->sync();
}

/**
 * 功 能：读字符串配置文件
 * 开发人员：Lzy     2013 - 七夕
 */
QString sys_configFile_readStr(QString strParameterName, QString strGroup)
{
    strParameterName = "/" + strGroup + "/" + strParameterName;

    sys_configFile_open();
    QString strParameter = pConfigIni->value(strParameterName).toString();
    sys_configFile_close();

    return strParameter;
}

/**
 * 功 能：读整形串配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
int sys_configFile_readInt(QString strParameterName, QString strGroup)
{       
    bool ok;

    int ret = sys_configFile_readStr(strParameterName, strGroup).toInt(&ok);
    if(!ok)  ret = -1;

    return ret;
}

/**
 * 功 能：读浮点形串配置文件
 * 开发人员：Lzy     2013 - 七夕
 */
double sys_configFile_readDouble(QString strParameterName, QString strGroup)
{
    bool ok;

    double ret = sys_configFile_readStr(strParameterName, strGroup).toDouble(&ok);
    if(!ok)  ret = -1;

    return ret;
}

/**
 * 功 能：参数写入配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
void sys_configFile_write(QString strParameterName, QString strParameter, QString strGroup)
{
    strParameterName = "/" + strGroup + "/" + strParameterName;
    pConfigIni->setValue(strParameterName, strParameter);
}

/**
 * 功 能：写入参数
 * 开发人员：Lzy     2016 - 七夕
 */
void sys_configFile_writeParam(QString name, QString value, QString strGroup)
{
    sys_configFile_open();
    sys_configFile_write(name, value, strGroup);
    sys_configFile_close();
}
