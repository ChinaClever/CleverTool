
INCLUDEPATH += $$PWD



HEADERS += \
    $$PWD/udp/recv/udprecvsocket.h \
    $$PWD/udp/sent/udpbdsent.h \
    $$PWD/udp/sent/udpheartbeat.h \
    $$PWD/udp/sent/udpsentsocket.h \
    $$PWD/tcp/client/tcpclient.h \
    $$PWD/tftp/tftp.h

	
SOURCES += \
    $$PWD/udp/recv/udprecvsocket.cpp \
    $$PWD/udp/sent/udpbdsent.cpp \
    $$PWD/udp/sent/udpheartbeat.cpp \
    $$PWD/udp/sent/udpsentsocket.cpp \
    $$PWD/tcp/client/tcpclient.cpp \
    $$PWD/tftp/tftp.cpp

FORMS += \





