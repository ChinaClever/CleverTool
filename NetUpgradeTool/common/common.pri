
INCLUDEPATH += $$PWD

include(ips/ips.pri)
include(excel/excel.pri)
include(exports/exports.pri)
include(tablecom/tablecom.pri)
include(network/network.pri)
include(upgrade/upgrade.pri)
include(iptables/iptables.pri)

HEADERS += \
    $$PWD/datapacket.h \
    $$PWD/msgbox.h

	
SOURCES += \
    $$PWD/datapacket.cpp \
    $$PWD/msgbox.cpp

FORMS += \





