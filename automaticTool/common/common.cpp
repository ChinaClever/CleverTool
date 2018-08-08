/*
 * common.cpp
 * 公共方法接口
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "common.h"
#include <QWidget>
#include <QHeaderView>
#include <time.h>
//#include <sys/time.h>
//#include <unistd.h>

/**
 * @brief 设置背景颜色
 * @param str 窗口标题名
 * @param target 目标窗口
 */
void com_setBackColour(const QString &str,QWidget *target)
{
    target->setWindowTitle(str);
    //target->setWindowFlags(Qt::FramelessWindowHint);
    target->setAutoFillBackground(true) ;
    QPalette paletteTool = target->palette() ;
    QBrush brushToolPic(QColor(WIDGET_BACK_COLOR)) ;
    QBrush brushToolwhite(QColor(BUTTON_BACK_COLOR)) ;
    paletteTool.setBrush(QPalette::Window,brushToolPic) ;
    paletteTool.setBrush(QPalette::Base,brushToolPic) ;
    paletteTool.setBrush(QPalette::Button,brushToolwhite) ;
    target->setPalette(paletteTool) ;

    target->setStyleSheet(BTN_FOCUS_BACK_COLOR);
//    target->setWindowIcon(QIcon(":/image/logo.jpg"));
}


/***
  * 获取程序数据目录
  */
QString cm_pathOfData(const QString& name)
{
    QDir dataDir(QDir::home());
    QString dirName = ".CleverTestSystem";
    if(!dataDir.exists(dirName))
        dataDir.mkdir(dirName);
    dataDir.cd(dirName);
    return dataDir.absoluteFilePath(name);
}

/***
  *判断一个字符串是否为纯数字
  */
bool cm_isDigitStr(const QString &src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s) //不是纯数字
        return false;

    return true;
}

/**
 * @brief 是否为IP地址
 * @param ip
 * @return
 */
bool cm_isIPaddress(const QString& ip)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(regExp.exactMatch(ip))
        return true ;

    return false;
}

/**
 * @brief 设备背景图标
 * @param widget 窗口
 * @param icon 图片路径
 */
void set_background_icon(QWidget *widget, const QString &icon)
{
    QPalette palette;
    QPixmap map = QPixmap(icon);
    palette.setBrush(QPalette::Background, QBrush(map));
    widget->setAutoFillBackground(true);
    widget->setPalette(palette);
}

/**
 * @brief 设备背景颜色
 * @param widget
 * @param color
 */
void set_background_color(QWidget *widget, const QColor &color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    widget->setAutoFillBackground(true);
    widget->setPalette(palette);
}


/**
 * @brief 设置背景图片
 * @param widget
 * @param icon
 * @param size
 */
void set_background_icon(QWidget *widget, const QString &icon,const QSize &size)
{    
    QPalette palette;
    QPixmap map = QPixmap(icon).scaled(size);
    palette.setBrush(QPalette::Background, QBrush(map));
    widget->setAutoFillBackground(true);
    widget->setPalette(palette);
}

/**
 * @brief button 样式
 */
void button_style_sheet(QWidget *target)
{
    target->setStyleSheet("QPushButton{background-color:rgb(102, 199, 249);}"
                          "QPushButton{font-family:'Microsoft YaHei';font-size:14px; color:rgb(0, 0, 0);}"
                          "QPushButton:hover{background-color:rgb(91, 237, 238);}"
                          "QPushButton:pressed{background-color:rgb(143, 185, 181);}"
                          "QComboBox{background-color:rgb(255,255,255);}" );
    target->setFont(QFont("微软雅黑",12));
}

/**
 * @brief button 样式 背景色为蓝色，字体为白色
 */
void btnBlue_style_sheet(QWidget *target)
{
    target->setStyleSheet("QPushButton{background-color:rgb(12,62,180); color:rgb(255, 255, 255);}"
                          "QPushButton:hover{background-color:rgb(91, 237, 238);color:rgb(0, 0, 0);}"
                          "QPushButton:pressed{background-color:rgb(143, 185, 181);color:rgb(0, 0, 0);}");
    target->setFont(QFont("微软雅黑",12));
}

/**
 * @brief 设置groupBox_background_icon
 * @param target
 */
void groupBox_background_icon(QWidget *target)
{
    set_background_icon(target,":/image/box_back.jpg");
    button_style_sheet(target);
}

/**
 * @brief 转16进制
 * @param array
 * @return
 */
QString cm_ByteArrayToHexStr(const QByteArray &array)
{
    QString strArray = array.toHex(); // 十六进制
    for(int i=0; i<array.size(); ++i)
        strArray.insert(2+3*i, " "); // 插入空格
    return strArray;
}

/**
 * @brief 转十进制
 * @param array
 * @return
 */
QString cm_ByteArrayToUcharStr(const QByteArray &array)
{
    QString strArray ;
    for(int i=0; i<array.size(); ++i)
        strArray += QString::number((uchar)array.at(i)) + " ";  // 十进制
    return strArray;
}



