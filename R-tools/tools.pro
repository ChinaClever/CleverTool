#-------------------------------------------------
#
# Project created by QtCreator 2016-11-17T17:45:52
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

RC_ICONS =pic.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tools
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    spcomm.cpp

HEADERS  += mainwindow.h \
    spcomm.h

FORMS    += mainwindow.ui

RESOURCES += \
    image.qrc

DISTFILES +=
