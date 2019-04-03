#-------------------------------------------------
#
# Project created by QtCreator 2018-12-12T08:51:25
#
#-------------------------------------------------

QT      += core gui
QT      += printsupport
QT      += sql
QT      += network
QT      += charts
QT      += serialport
QT      += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = M-NetUpgradeTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(tftp/tftp.pri)
include(common/excel/excel.pri)
include(common/tablecom/tablecom.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    common\msgbox.cpp \
    common\datapacket.cpp \
    export/exportdlg.cpp \
    bulidip/ipswid.cpp \
    tables/ipstabwid.cpp \
    tables/errstabwid.cpp \
    tables/okstabwid.cpp \
    udp/sent/udpbdsent.cpp \
    udp/sent/udpheartbeat.cpp \
    udp/sent/udpsentsocket.cpp \
    udp/recv/udprecvsocket.cpp \
    udp/udptesting.cpp

HEADERS += \
        mainwindow.h \
    common\msgbox.h \
    common\datapacket.h \
    export/exportdlg.h \
    bulidip/ipswid.h \
    tables/ipstabwid.h \
    tables/errstabwid.h \
    tables/okstabwid.h \
    udp/sent/udpbdsent.h \
    udp/sent/udpheartbeat.h \
    udp/sent/udpsentsocket.h \
    udp/recv/udprecvsocket.h \
    udp/udptesting.h
RESOURCES += \
    images/images.qrc

FORMS += \
        mainwindow.ui \
    export/exportdlg.ui \
    bulidip/ipswid.ui

RC_ICONS = logo.ico

