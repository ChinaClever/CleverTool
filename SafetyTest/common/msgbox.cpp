/*
 * msgbox.cpp
 * 信息提示框
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "msgbox.h"

extern void com_setBackColour(const QString &str,QWidget *target);

QuMsgBox::QuMsgBox(QWidget *parent,QString strContext) : QMessageBox(parent)
{
    //    setWindowFlags(Qt::WindowStaysOnTopHint);
    //    setWindowTitle(tr("提示信息"));
//    com_setBackColour(tr("提示信息"),this);
//    setWindowIcon(QIcon(":/images/logo.jpg"));
    resize(1000, 600);

    setIcon(QMessageBox::Question);
    setText(strContext);

    confirmBut = addButton(tr("确定"), QMessageBox::AcceptRole);
    cancelBut = addButton(tr("取消"), QMessageBox::AcceptRole);
    confirmBut->setFixedSize(60,25);
    cancelBut->setFixedSize(60,25);
    setDefaultButton(confirmBut);

    cancelBut->setFocus();
    confirmBut->setFocus();

    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);} QWidget{font: 15pt \"微软雅黑\";}" );
}

QuMsgBox::~QuMsgBox()
{
}

bool QuMsgBox::Exec(void)
{
    exec();
    if( clickedButton() == dynamic_cast<QAbstractButton *>(confirmBut))
        return true ;
    else if(clickedButton() == dynamic_cast<QAbstractButton *>(cancelBut))
        return false ;
    return false ;
}


WaringMsgBox::WaringMsgBox(QWidget *parent,QString strContext) : QMessageBox(parent)
{
    //    setWindowFlags(Qt::WindowStaysOnTopHint);
    //    setWindowTitle(tr("警告信息"));
//    com_setBackColour(tr("警告信息"),this);
//    setWindowIcon(QIcon(":/images/logo.jpg"));
    resize(500, 400);

    setIcon(QMessageBox::Warning);
    setText(strContext);

    confirmBut = addButton(tr("确定"), QMessageBox::AcceptRole);
    cancelBut = addButton(tr("取消"), QMessageBox::AcceptRole);
    confirmBut->setMinimumSize(75,29);
    cancelBut->setMinimumSize(75,29);

    setDefaultButton(confirmBut);

    confirmBut->setFocus();
    cancelBut->setFocus();
    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);}" );
}

WaringMsgBox::~WaringMsgBox()
{
}

bool WaringMsgBox::Exec(void)
{
    exec();
    if( clickedButton() == dynamic_cast<QAbstractButton *>(confirmBut))
        return true ;
    else if(clickedButton() == dynamic_cast<QAbstractButton *>(cancelBut))
        return false ;
    return false ;
}


InfoMsgBox::InfoMsgBox(QWidget *parent,QString strContext) :
    QMessageBox(parent)
{
    //    setWindowFlags(Qt::WindowStaysOnTopHint);
    //    setWindowTitle(tr("信息提示"));
//    com_setBackColour(tr("信息提示"),this);
//    setWindowIcon(QIcon(":/images/logo.jpg"));
    resize(500, 400);

    setIcon(QMessageBox::Information);
    setText(strContext);

    confirmBut = addButton(tr("确定"), QMessageBox::AcceptRole);
    setDefaultButton(confirmBut);
    confirmBut->setMinimumSize(75,29);

    confirmBut->setFocus();
    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);}" );

    //QTimer::singleShot(AutoOutTime*1000,this,SLOT(close())); 自动关闭

    exec(); /* 自动 运行*/
}

InfoMsgBox::~InfoMsgBox()
{
}


CriticalMsgBox::CriticalMsgBox(QWidget *parent,QString strContext) :
    QMessageBox(parent)
{
    //    setWindowFlags(Qt::WindowStaysOnTopHint);
    //    setWindowTitle(tr("错误提示"));
//    com_setBackColour(tr("错误提示"),this);
//    setWindowIcon(QIcon(":/images/logo.jpg"));
    resize(500, 400);

    setIcon(QMessageBox::Critical);
    setText(strContext);

    confirmBut = addButton(tr("确定"), QMessageBox::AcceptRole);
    setDefaultButton(confirmBut);
    confirmBut->setMinimumSize(75,29);

    confirmBut->setFocus();
    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);}" );

    //QTimer::singleShot(AutoOutTime*1000,this,SLOT(close())); 自动关闭

    exec(); /* 自动 运行*/
}

CriticalMsgBox::~CriticalMsgBox()
{
}

