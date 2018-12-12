
INCLUDEPATH += $$PWD

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/Release/ -llibssh2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/Debug/ -llibssh2

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/lib

HEADERS += \


	
SOURCES += \


FORMS += \





