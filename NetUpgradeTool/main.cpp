﻿#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    a.installTranslator(w.qtrans);
    return a.exec();
}
