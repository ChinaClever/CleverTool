#ifndef ACTIVECODE_H
#define ACTIVECODE_H
#include "machinecode.h"
#include <QDate>

/* 注册状态码定义 */
enum {
    ACTIVE_RET_UNREGISTERED, // 未注册
    ACTIVE_RET_FREE, // 免费使用
    ACTIVE_RET_OK, // 注册成功
    ACTIVE_RET_EXPIRYDATE, // 日期失效
    ACTIVE_RET_ERR // 注册码错误
};

class ActiveCode
{
public:
    ActiveCode();

    QString createActiveCode(const QDate &date, int count, const QString &str);
    bool verifyCode(const QDate &date, int count, const QString &machineCode,
                    const QString &activeCode);

    int verifyLocalCode();

};

#endif // ACTIVECODE_H
