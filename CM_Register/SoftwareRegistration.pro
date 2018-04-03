#-------------------------------------------------
#
# Project created by QtCreator 2016-10-29T13:50:49
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoftwareRegistration
TEMPLATE = app

include(register/register.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    we.qrc
