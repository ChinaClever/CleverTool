
INCLUDEPATH += $$PWD


HEADERS += \
    $$PWD/src/AbstractJob.h \
    $$PWD/src/defines.h \
    $$PWD/src/QtSnmpData.h \
    $$PWD/src/RequestSubValuesJob.h \
    $$PWD/src/RequestValuesJob.h \
    $$PWD/src/Session.h \
    $$PWD/src/SetValueJob.h \
    $$PWD/src/win_export.h \
    $$PWD/src/QtSnmpClient.h \
    $$PWD/snmpthread.h

	
SOURCES += \
    $$PWD/src/AbstractJob.cpp \
    $$PWD/src/QtSnmpData.cpp \
    $$PWD/src/RequestSubValuesJob.cpp \
    $$PWD/src/RequestValuesJob.cpp \
    $$PWD/src/Session.cpp \
    $$PWD/src/SetValueJob.cpp \
    $$PWD/src/QtSnmpClient.cpp \
    $$PWD/snmpthread.cpp

FORMS += \





