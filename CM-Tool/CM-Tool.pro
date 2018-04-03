#-------------------------------------------------
#
# Project created by QtCreator 2017-04-01T17:01:04
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CM-Tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    udp/heartbeat.cpp \
    udp/datasouce.cpp \
    udp/datasent.cpp \
    udp/myudpthread.cpp \
    tcp/tcpsocket.cpp \
    tcp/tcpserver.cpp \
    tcp/tcpthread.cpp \
    tcp/analysisdata.cpp \
    thresholdsetting/settingdialog.cpp \
    udp/globalreceivethread.cpp


HEADERS  += mainwindow.h \
    udp/heartbeat.h \
    udp/datadefine.h \
    udp/datasouce.h \
    udp/datasent.h \
    udp/myudpthread.h \
    tcp/tcpsocket.h \
    tcp/tcpserver.h \
    tcp/tcpthread.h \
    tcp/analysisdata.h \
    thresholdsetting/settingdialog.h \
    udp/globalreceivethread.h

FORMS    += mainwindow.ui \
    thresholdsetting/settingdialog.ui

RESOURCES += \
    image/image.qrc
