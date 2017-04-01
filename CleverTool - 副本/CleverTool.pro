#-------------------------------------------------
#
# Project created by QtCreator 2017-03-13T13:58:06
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CleverTool
TEMPLATE = app

include(adjust/adjust.pri)
include(serialport/serialport.pri)

SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \
    datadefine.h

FORMS    += mainwindow.ui

RESOURCES += \
    image.qrc
