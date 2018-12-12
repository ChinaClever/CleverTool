
INCLUDEPATH += $$PWD

include(curl/curl.pri)

HEADERS += \	
    $$PWD/tftp.h \
    $$PWD/tftpthread.h \
    $$PWD/tftpwidget.h
    
   
	
SOURCES += \	
    $$PWD/tftp.cpp \
    $$PWD/tftpthread.cpp \
    $$PWD/tftpwidget.cpp
    
    

FORMS += \
    $$PWD/tftpwidget.ui


