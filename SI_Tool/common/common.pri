
INCLUDEPATH += $$PWD

HEADERS += \	
    $$PWD/common.h \
    $$PWD/msgbox.h \
    common/sysconfigfile.h \
    $$PWD/configbase.h \
    $$PWD/comtablewid.h
#    common/qcustomplot.h \
	
SOURCES += \	
    $$PWD/common.cpp \
    $$PWD/msgbox.cpp \
    common/sysconfigfile.cpp \
    $$PWD/configbase.cpp \
    $$PWD/comtablewid.cpp
#    common/qcustomplot.cpp \

FORMS += \
    $$PWD/comtablewid.ui




