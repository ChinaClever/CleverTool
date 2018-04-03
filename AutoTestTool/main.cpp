#include "mainwindow.h"
#include <QApplication>

#include<QSqlDriver>
#include<QSqlDatabase>
#include<QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    qDebug()<<"Available drivers:";
//    QStringList drivers=QSqlDatabase::drivers();
//    foreach(QString driver,drivers)
//        qDebug()<<driver;

    return a.exec();
}
