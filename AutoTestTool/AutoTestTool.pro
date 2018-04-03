#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T17:55:22
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += network
QT       += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoTestTool
TEMPLATE = app

DESTDIR = $$PWD/bin


SOURCES += main.cpp\
        mainwindow.cpp \
    json/json.cpp \
    json/mysql.cpp \
    mytext.cpp \
    seriorport/myseriorport.cpp \
    seriorport/datapacket.cpp

HEADERS  += mainwindow.h \   
    json/json.h \
    json/mysql.h \
    mytext.h \
    datadefine.h \
    seriorport/myseriorport.h \
    seriorport/datapacket.h

FORMS    += mainwindow.ui \
    mytext.ui

RESOURCES += \
    image.qrc
