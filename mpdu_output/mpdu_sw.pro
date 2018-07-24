#-------------------------------------------------
#
# Project created by QtCreator 2018-06-06T09:15:51
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mpdu_output
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

include(common/common.pri)
include(serialport/serialport.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    modubs_rtu/si_rtu_sent.cpp \
    modubs_rtu/si_rtuthread.cpp \
    mpdu_modbus_test/mpdu_test_sw/mpdu_outputswitemwid.cpp \
    mpdu_modbus_test/mpdu_test_sw/mpdu_outputswwid.cpp \
    mpdu_modbus_test/mpdu_test_threshold/mpdu_outputthresholditemwid.cpp \
    mpdu_modbus_test/mpdu_test_threshold/mpdu_outputthresholdwid.cpp

HEADERS += \
        mainwindow.h \
    modubs_rtu/si_rtu_sent.h \
    modubs_rtu/si_rtuthread.h \
    mpdu_modbus_test/mpdu_test_sw/mpdu_outputswitemwid.h \
    mpdu_modbus_test/mpdu_test_sw/mpdu_outputswwid.h \
    mpdu_modbus_test/mpdu_test_threshold/mpdu_outputthresholditemwid.h \
    mpdu_modbus_test/mpdu_test_threshold/mpdu_outputthresholdwid.h

FORMS += \
        mainwindow.ui \
    mpdu_modbus_test/mpdu_test_sw/mpdu_outputswitemwid.ui \
    mpdu_modbus_test/mpdu_test_sw/mpdu_outputswwid.ui \
    mpdu_modbus_test/mpdu_test_threshold/mpdu_outputthresholditemwid.ui \
    mpdu_modbus_test/mpdu_test_threshold/mpdu_outputthresholdwid.ui
