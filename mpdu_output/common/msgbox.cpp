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
    com_setBackColour(tr("提示信息"),this);

    setIcon(QMessageBox::Question);
    setText(strContext);

    confirmBut = addButton(tr("确定"), QMessageBox::AcceptRole);
    cancelBut = addButton(tr("取消"), QMessageBox::AcceptRole);
    confirmBut->setFixedSize(60,25);
    cancelBut->setFixedSize(60,25);
    setDefaultButton(confirmBut);

    cancelBut->setFocus();
    confirmBut->setFocus();

    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);}" );
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
    com_setBackColour(tr("警告信息"),this);

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
    com_setBackColour(tr("信息提示"),this);
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
    com_setBackColour(tr("错误提示"),this);

    setIcon(QMessageBox::Critical);
    setText(strContext);

    confirmBut = addButton(tr("确定"), QMessageBox::AcceptRole);
    setDefaultButton(confirmBut);
    confirmBut->setMinimumSize(75,29);

    confirmBut->setFocus();
    setStyleSheet( "QPushButton:hover{background-color:rgb(91, 237, 238);}" );

//    QTimer::singleShot(AutoOutTime*1000,this,SLOT(close())); 自动关闭

    exec(); /* 自动 运行*/
}

CriticalMsgBox::~CriticalMsgBox()
{
}

