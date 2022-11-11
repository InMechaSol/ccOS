## ccOS depends on the ccNOos project
## if the ccNOos project hasn't already been loaded elsewhere
## it should be loaded here, now
if(!defined(ccNOos_CPPBuild,test)) {
    if(!defined(ccNOosDIR,var)) {
        error("ccNOosDIR is not defined")
    }
    include($$ccNOosDIR/ccNOos.pri)
}

INCLUDEPATH += $$ccOSDIR/osExecutionSystem
INCLUDEPATH += $$ccOSDIR/exeThread
INCLUDEPATH += $$ccOSDIR/serialComms
INCLUDEPATH += $$ccOSDIR/tcpComms
INCLUDEPATH += $$ccOSDIR/udpComms
INCLUDEPATH += $$ccOSDIR/stdIOExtProc

HEADERS += $$ccOSDIR/osExecutionSystem/version_config.hpp
HEADERS += $$ccOSDIR/osExecutionSystem/os_execution_system.hpp
HEADERS += $$ccOSDIR/exeThread/exe_thread.hpp

HEADERS += $$ccOSDIR/serialComms/serial_comms.h
HEADERS += $$ccOSDIR/serialComms/serial_comms.c
HEADERS += $$ccOSDIR/tcpComms/tcp_comms.h
HEADERS += $$ccOSDIR/tcpComms/tcp_comms.c
HEADERS += $$ccOSDIR/udpComms/udp_comms.hpp
HEADERS += $$ccOSDIR/stdIOExtProc/stdio_extproc.h
HEADERS += $$ccOSDIR/stdIOExtProc/stdio_extproc.c


SOURCES += $$ccOSDIR/osExecutionSystem/os_execution_system.cpp
SOURCES += $$ccOSDIR/exeThread/exe_thread.cpp
SOURCES += $$ccOSDIR/serialComms/serial_comms_class.cpp
SOURCES += $$ccOSDIR/tcpComms/tcp_comms_class.cpp
SOURCES += $$ccOSDIR/udpComms/udp_comms.cpp
SOURCES += $$ccOSDIR/stdIOExtProc/stdio_extproc.cpp

if(ccOS_menuClient) {
	INCLUDEPATH += $$ccOSDIR/menuAPIclient
	HEADERS += $$ccOSDIR/menuAPIclient/menuAPIclient.hpp
	SOURCES += $$ccOSDIR/menuAPIclient/menuAPIclient.cpp
}


if(ccOSAllLibs|ccOSLibs_WMM) {
    INCLUDEPATH += $$ccOSDIR/ccLibs/acs/NOAA_WMM
    HEADERS += $$ccOSDIR/ccLibs/acs/NOAA_WMM/GeomagnetismHeader.h
    SOURCES += $$ccOSDIR/ccLibs/acs/NOAA_WMM/GeomagnetismHeader.c
}

if(ccOSAllDevs|ccOSDevs_ft232h) {
    INCLUDEPATH += $$ccOSDIR/ccLibs/devices/adafruit_ft232h
    HEADERS += $$ccOSDIR/ccLibs/devices/adafruit_ft232h/adafruit_ft232h.h
    HEADERS += $$ccOSDIR/ccLibs/devices/adafruit_ft232h/adafruit_ft232h.c
    SOURCES += $$ccOSDIR/ccLibs/devices/adafruit_ft232h/adafruit_ft232h.cpp
}
