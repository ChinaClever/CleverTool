#ifndef COMMON_H
#define COMMON_H
#include <QtCore>
#include "msgbox.h"
#include "configbase.h"

//const QColor gray(174,225,254);//空设备,原为灰色，现为天蓝色
//const QColor green(97,202,40);//正常
//const QColor orange(255,0,0);//报警，原为橘红，现为红色
//const QColor blue(200,200,200);//离线，原为男色，现为灰色

/* 窗口背景色 */
#define     WIDGET_BACK_COLOR   232,249,255
#define     BUTTON_BACK_COLOR  232,238,250

#define     DILAG_BACK_COLOR      85,152,215
#define     DILAG_BUTTON_COLOR  255,255,255

/* 获得焦点BTN背景颜色 */
#define    BTN_FOCUS_BACK_COLOR    "QPushButton:focus{background-color: rgb(96,238,250);}"

/* 设置窗口背景色函数 */
extern void com_setBackColour(const QString &,QWidget *target);

// 倍率定义
#define COM_RATE_VOL	1.0    // 电压
#define COM_RATE_CUR	10.0    // 电流
#define COM_RATE_POW	1000.0  // 功率
#define COM_RATE_ELE	10.0    // 电能
#define COM_RATE_PF     100.0   // 功率因素
#define COM_RATE_TEM	1.0    // 温度
#define COM_RATE_HUM	1.0    // 湿度


QString cm_pathOfData(const QString& name);
bool cm_isDigitStr(const QString &src);
bool cm_isIPaddress(const QString& ip);

QString cm_ByteArrayToHexStr(const QByteArray &array);
QString cm_ByteArrayToUcharStr(const QByteArray &array);

void btnBlue_style_sheet(QWidget *target);
void groupBox_background_icon(QWidget *target);
void set_background_icon(QWidget *widget, const QString &icon);
void set_background_color(QWidget *widget, const QColor &color);
void set_background_icon(QWidget *widget, const QString &icon, const QSize &size);

#endif // COMMON_H
