
INCLUDEPATH += $$PWD

include(snmplib/snmplib.pri)

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib

HEADERS += \
    $$PWD/lib/QtSnmpClient.h \
    $$PWD/lib/QtSnmpData.h \
    $$PWD/snmpthread.h

	
SOURCES += \
    $$PWD/snmpthread.cpp

FORMS += \





