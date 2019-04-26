#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T10:52:24
#
#-------------------------------------------------

QT  += core gui
QT  += axcontainer

TARGET = icom
TEMPLATE = lib

DEFINES += ICOM_LIBRARY

SOURCES += icom.cpp

HEADERS += icom.h\
        icom_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
