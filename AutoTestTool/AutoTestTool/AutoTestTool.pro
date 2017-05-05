#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T17:55:22
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoTestTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    udp/udpclient.cpp \
    tcp/tcpclient.cpp \
    tcp/cmdtonetpacket.cpp \
    json/json.cpp \
    udp/analysisdata.cpp

HEADERS  += mainwindow.h \
    udp/udpclient.h \
    tcp/tcpclient.h \
    datadefine.h \
    tcp/cmdtonetpacket.h \
    json/json.h \
    udp/analysisdata.h

FORMS    += mainwindow.ui
