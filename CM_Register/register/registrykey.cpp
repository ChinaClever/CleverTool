/**
 * registrykey.cpp
 * 注册表操作
 *  注册表保存四个值，失效日期，管理数量，机器码，激活码
 *
 *  Created on: 2016年10月29日
 *      Author: Lzy
 */
#include "registrykey.h"
#include <QDateTime>

#define SOFT_REG_KEY "HKEY_CURRENT_USER\\Software\\qCleverManager"
#define KEY_DATE    "date"
#define KEY_COUNT   "count"
#define KEY_MACHINE "machine"
#define KEY_CODE    "activeCode"

RegistryKey::RegistryKey()
{

}

/**
 * @brief 有效时间
 */
void RegistryKey::writeDate(const QDate &date)
{
    QString str = date.toString("yyyy-MM-dd");
    write(KEY_DATE, str);
}


/**
 * @brief 写台数
 */
void RegistryKey::writeCount(int count)
{
    QString str = QString::number(count);
    write(KEY_COUNT, str);
}

void RegistryKey::writeMachineCode(const QString &code)
{
    write(KEY_MACHINE, code);
}

/**
 * @brief 保存激活码
 */
void RegistryKey::writeActiveCode(const QString &code)
{
    write(KEY_CODE, code);
}

/**
 * @brief 读取失效日期
 */
QDate RegistryKey::readDate()
{
    QString str = read(KEY_DATE);
    QDate date = QDate::fromString(str, "yyyy-MM-dd");
    return date;
}

/**
 * @brief 读取设备数量
 * @return -1 未注册
 */
int RegistryKey::readCount()
{
    int count = -1;
    QString str = read(KEY_COUNT);
    if(!str.isEmpty())
        count = str.toInt();

    return count;
}

QString RegistryKey::readMachineCode()
{
    return read(KEY_MACHINE);
}

/**
 * @brief 读取激活码
 */
QString RegistryKey::readActiveCode()
{
    return read(KEY_CODE);
}

/**
 * @brief 写注册表表
 */
void RegistryKey::write(const QString &key, const QString &str)
{
    if(!str.isEmpty()) {
        QSettings *reg = new QSettings(SOFT_REG_KEY,QSettings::NativeFormat);
        reg->setValue(key,str);
        delete reg;
    }
}

/**
 * @brief 读取注册码
 */
QString RegistryKey::read(const QString &key)
{
    QSettings *reg = new QSettings(SOFT_REG_KEY,QSettings::NativeFormat);
    QString activeCode = reg->value(key).toString();
    delete reg;

    return activeCode;
}
