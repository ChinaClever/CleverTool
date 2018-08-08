#-------------------------------------------------
#
# Project created by QtCreator 2018-07-25T16:39:02
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZPDU-Tool
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS

include(serialport/serialport.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h \
    datadefine.h

FORMS += \
        mainwindow.ui
