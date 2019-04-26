
INCLUDEPATH += $$PWD

win32:CONFIG(release, debug|release): LIBS += -L$$PWD -lquc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD -lqucd

#include(include.pri)

HEADERS += \
	
SOURCES += \

FORMS += \





