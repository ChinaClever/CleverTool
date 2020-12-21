#ifndef MSGBOX_H
#define MSGBOX_H

#include <QObject>
#include <QtGui>
#include <QMessageBox>
#include <QPushButton>
#include <QInputDialog>

/*询问消息框*/
class QuMsgBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit QuMsgBox(QWidget *parent = 0,  QString strContext = "");
    ~QuMsgBox();
    bool Exec(void);

private:
     QPushButton *confirmBut,*cancelBut;
};

/*警告消息框*/
class WaringMsgBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit WaringMsgBox(QWidget *parent = 0,  QString strContext = "");
    ~WaringMsgBox();
    bool Exec(void);

private:
     QPushButton *confirmBut,*cancelBut;
};

/*消息提示框*/
class InfoMsgBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit InfoMsgBox(QWidget *parent = 0,  QString strContext = "");
    ~InfoMsgBox();

private:
    QPushButton *confirmBut;
};

/*错误提示框*/
class CriticalMsgBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit CriticalMsgBox(QWidget *parent = 0,  QString strContext = "");
    ~CriticalMsgBox();

private:
    QPushButton *confirmBut;
};


#endif // MSGBOX_H
