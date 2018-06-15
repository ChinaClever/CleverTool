
INCLUDEPATH += $$PWD

CONFIG += qaxcontainer


INCLUDEPATH += $$REGISTERLIB_LIBRARY
DEPENDPATH += $$REGISTERLIB_LIBRARY


HEADERS += \
    $$PWD/registrykey.h \
    $$PWD/machinecode.h \
    $$PWD/softwareregiste.h \
    $$PWD/activecode.h


    

SOURCES += \
    $$PWD/registrykey.cpp \
    $$PWD/machinecode.cpp \
    $$PWD/activecode.cpp


    

