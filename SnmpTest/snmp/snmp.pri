
INCLUDEPATH += $$PWD

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lqtsnmpclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/d/ -lqtsnmpclient

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib

HEADERS += \
    $$PWD/lib/QtSnmpClient.h \
    $$PWD/lib/QtSnmpData.h \
    $$PWD/snmpthread.h

	
SOURCES += \
    $$PWD/snmpthread.cpp

FORMS += \





