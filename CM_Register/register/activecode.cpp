/**
 * ActiveCode.cpp
 * 激活码：由失效日期，设备数量，机器码，生成激活码
 *
 * 此类提供三个接口方法：生成激活码，验证激活码，验证本地激活码
 *
 *  Created on: 2016年10月29日
 *      Author: Lzy
 */
#include "activecode.h"

ActiveCode::ActiveCode()
{

}

/**
 * @brief 生成激活码
 * @param date 失效日期
 * @param count 设备数量
 * @param str
 * @return
 */
QString ActiveCode::createActiveCode(const QDate &date, int count, const QString &str)
{
    QString strDate = date.toString("yyyy-MM-dd");
    QString strCount = QString::number(count);

    QString machineCode = strDate + strCount + str;
    QCryptographicHash sha1(QCryptographicHash::Sha1); // MD4加密
    QByteArray datagram(machineCode.toLatin1());
    const char* tempConstChar = datagram.data();
    sha1.addData(tempConstChar);
    QString  activeCode=sha1.result().toHex();

    QClipboard *board = QApplication::clipboard();
    board->setText(activeCode);

    return activeCode;
}

/**
 * @brief 验证激活码
 * @return true
 */
bool ActiveCode::verifyCode(const QDate &date, int count, const QString &machineCode,
                const QString &activeCode)
{
    bool ret = true;
    QString str = createActiveCode(date, count, machineCode);
    if(str != activeCode) {
        ret = false;
    }
    return ret;
}

/**
 * @brief 验证本机激活码
 */
int ActiveCode::verifyLocalCode()
{
    RegistryKey key;
    int ret = ACTIVE_RET_UNREGISTERED; // 未注册

    int count = key.readCount();
    if(count > 5)  // 大于5 需要验证激活
    {
        QDate date = key.readDate();
        if(date > QDate::currentDate()) { //失效日期要比当前日期早
            QString machineCode = key.readMachineCode();
            QString activeCode = key.readActiveCode();
            bool r = verifyCode(date, count, machineCode, activeCode);
            if(r)
                ret = ACTIVE_RET_OK; //激活码正确
            else
                ret = ACTIVE_RET_ERR; // 激活码错误
        } else {
            ret = ACTIVE_RET_EXPIRYDATE; // 日期失效
        }
    } else if(count == -1){ // 未注册
        ret = ACTIVE_RET_UNREGISTERED;
    } else
        ret = ACTIVE_RET_FREE; // 免费使用

    return ret;
}
