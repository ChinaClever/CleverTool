#ifndef SOFTWAREREGISTE_H
#define SOFTWAREREGISTE_H
/**
 * 库类声明
 *
 *  Created on: 2016年10月29日
 *      Author: Lzy
 */
#include <QString>
#include <QtCore>

class Q_DECL_IMPORT SoftwareRegiste
{
public:
    SoftwareRegiste();
    QString getWMIHWInfo(int type);
};

#endif // SOFTWAREREGISTE_H
