#-------------------------------------------------
#
# Project created by QtCreator 2019-04-03T14:26:10
#
#-------------------------------------------------

QT       += core gui
QT      += printsupport
QT      += sql
QT      += network
QT      += charts
QT      += serialport
QT      += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetUpgradeTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#TRANSLATIONS = English.ts
TRANSLATIONS += english.ts chinese.ts
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(common/common.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    udptesting.cpp \
    devselectwid.cpp

HEADERS += \
        mainwindow.h \
    udptesting.h \
    devselectwid.h

FORMS += \
        mainwindow.ui \
    devselectwid.ui


RESOURCES += \
    images/images.qrc

RC_ICONS = logo.ico
