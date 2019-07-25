
INCLUDEPATH += $$PWD
include(excel/excel.pri)
HEADERS += \	
    $$PWD/common.h \
    $$PWD/msgbox.h \
    $$PWD/sysconfigfile.h \
    $$PWD/configbase.h \
    $$PWD/comtablewid.h
	
SOURCES += \	
    $$PWD/common.cpp \
    $$PWD/msgbox.cpp \
    $$PWD/sysconfigfile.cpp \
    $$PWD/configbase.cpp \
    $$PWD/comtablewid.cpp

FORMS += \
    $$PWD/comtablewid.ui




