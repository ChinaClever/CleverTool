INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/basicExcel/

win32: {
    QT += axcontainer
    LIBS += -L$$PWD/lib/ -licom
#    CONFIG += qaxcontainer
} else: {
}


DEPENDPATH += $$PWD


HEADERS += \
    $$PWD/basicExcel/qexcel.h \
    $$PWD/basicExcel/BasicExcel.hpp \
    $$PWD/excel_savethread.h

SOURCES += \
    $$PWD/basicExcel/qexcel.cpp \
    $$PWD/basicExcel/BasicExcel.cpp \
    $$PWD/execl_demo.cpp \
    $$PWD/excel_savethread.cpp

