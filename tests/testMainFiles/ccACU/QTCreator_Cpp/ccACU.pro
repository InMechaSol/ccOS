TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
DEFINES += PLATFORM_ccOS
QMAKE_LFLAGS += -lrt -lpthread
QMAKE_EXT_CPP = .cpp
QMAKE_EXT_H = .h .hpp .c

#INCLUDEPATH += \
#    ../../../../ccLibs/devices/adafruit_ft232h \
#    ../../../../ccNOos/ccLibs/acs \
#    ../../../../ccNOos/ccLibs/devices/ADRF6650 \
#    ../../../../ccNOos/ccLibs/devices/ADRF6650 \
#    ../../../../ccNOos/ccLibs/devices/HMC1119 \
#    ../../../../ccNOos/ccLibs/devices/HMR3300 \
#    ../../../../ccNOos/ccLibs/devices/LTC2360 \
#    ../../../../ccNOos/ccLibs/devices/MCP23017 \
#    ../../../../ccNOos/ccLibs/devices/NEO_LEA_M8T \
#    ../../../../ccNOos/ccLibs/mcs \
#    ../../../../ccNOos/computeModule \
#    ../../../../ccNOos/consoleMenu \
#    ../../../../ccNOos/executionSystem \
#    ../../../../ccNOos/executionSystem \
#    ../../../../ccNOos/ioDevice \
#    ../../../../ccNOos/tests/testApps/SatComACS \
#    ../../../../ccNOos/tests/testApps/SatComACS \
#    ../../../../ccNOos/tests/testApps/SatComACS/apiModules/ConsoleMenu \
#    ../../../../ccNOos/tests/testApps/SatComACS/apiModules/LCDKeyPad \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/APT_WMM \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/TPM \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/TxRx \
#    ../../../../ccNOos/tests/testPlatforms \
#    ../../../../exeThread \
#    ../../../../osExecutionSystem \
#    ../../../../serialComms \
#    ../../../../stdIOExtProc \
#    ../../../../tcpComms \
#    ../../../testApps/ccACU/apiModules/CGI_ServerClass \
#    ../../../testApps/ccACU/apiModules/SNMP_AgentsServer \
#    ../../../testApps/ccACU/apiModules/UI_ServerClass \
#    ../../../testApps/ccACU/deviceModules/APT_WMM \
#    ../../../testApps/ccACU/deviceModules/ManagedSwitch \
#    ../../../testApps/ccACU/deviceModules/Modem \
#    ../../../testApps/ccACU/deviceModules/TPM \
#    ../../../testApps/ccACU/deviceModules/TxRx \
#    ../../../testApps/ccACU

#HEADERS += \
#    ../../../../ccLibs/devices/adafruit_ft232h/adafruit_ft232h.c \
#    ../../../../ccNOos/ccLibs/acs/satComControl.c \
#    ../../../../ccNOos/ccLibs/devices/ADRF6650/ADRF6650.c \
#    ../../../../ccNOos/ccLibs/devices/HMC1119/HMC1119.c \
#    ../../../../ccNOos/ccLibs/devices/HMR3300/HMR3300.c \
#    ../../../../ccNOos/ccLibs/devices/LTC2360/LTC2360.c \
#    ../../../../ccNOos/ccLibs/devices/MCP23017/MCP23017.c \
#    ../../../../ccNOos/ccLibs/devices/NEO_LEA_M8T/NEO_LEA_M8T.c \
#    ../../../../ccNOos/ccLibs/mcs/motionControl.c \
#    ../../../../ccNOos/computeModule/compute_module.c \
#    ../../../../ccNOos/consoleMenu/console_menu.c \
#    ../../../../ccNOos/executionSystem/execution_system.c \
#    ../../../../ccNOos/ioDevice/io_device.c \
#    ../../../../ccNOos/tests/testApps/SatComACS/Application_Solution.c \
#    ../../../../ccNOos/tests/testApps/SatComACS/PlatformApp_Serialization.c \
#    ../../../../ccNOos/tests/testApps/SatComACS/apiModules/ConsoleMenu/MenuAPI.c \
#    ../../../../ccNOos/tests/testApps/SatComACS/apiModules/LCDKeyPad/LCDapi.c \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/APT_WMM/APTmodule.c \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/TPM/TPMmodule.c \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/TxRx/TxRxmodule.c \
#    ../../../../serialComms/serial_comms.c \
#    ../../../../stdIOExtProc/stdio_extproc.c \
#    ../../../../tcpComms/tcp_comms.c \
#    ../../../../ccLibs/devices/adafruit_ft232h/adafruit_ft232h.h \
#    ../../../../ccNOos/ccLibs/acs/satComControl.h \
#    ../../../../ccNOos/ccLibs/devices/ADRF6650/ADRF6650.h \
#    ../../../../ccNOos/ccLibs/devices/ADRF6650/ADRF6650_Map.h \
#    ../../../../ccNOos/ccLibs/devices/HMC1119/HMC1119.h \
#    ../../../../ccNOos/ccLibs/devices/HMR3300/HMR3300.h \
#    ../../../../ccNOos/ccLibs/devices/LTC2360/LTC2360.h \
#    ../../../../ccNOos/ccLibs/devices/MCP23017/MCP23017.h \
#    ../../../../ccNOos/ccLibs/devices/NEO_LEA_M8T/NEO_LEA_M8T.h \
#    ../../../../ccNOos/ccLibs/mcs/motionControl.h \
#    ../../../../ccNOos/computeModule/compute_module.h \
#    ../../../../ccNOos/consoleMenu/console_menu.h \
#    ../../../../ccNOos/executionSystem/execution_system.h \
#    ../../../../ccNOos/executionSystem/version_config.h \
#    ../../../../ccNOos/ioDevice/io_device.h \
#    ../../../../ccNOos/tests/testApps/SatComACS/Application_Solution.h \
#    ../../../../ccNOos/tests/testApps/SatComACS/PlatformApp_Serialization.h \
#    ../../../../ccNOos/tests/testApps/SatComACS/apiModules/ConsoleMenu/MenuAPI.h \
#    ../../../../ccNOos/tests/testApps/SatComACS/apiModules/LCDKeyPad/LCDapi.h \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/APT_WMM/APTmodule.h \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/TPM/TPMmodule.h \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/TxRx/TxRxmodule.h \
#    ../../../../ccNOos/tests/testPlatforms/Platform_ccOS.hpp \
#    ../../../../exeThread/exe_thread.hpp \
#    ../../../../osExecutionSystem/os_execution_system.hpp \
#    ../../../../osExecutionSystem/version_config.hpp \
#    ../../../../serialComms/serial_comms.h \
#    ../../../../stdIOExtProc/stdio_extproc.h \
#    ../../../../tcpComms/tcp_comms.h \
#    ../../../testApps/ccACU/apiModules/CGI_ServerClass/CGIServerClass.hpp \
#    ../../../testApps/ccACU/apiModules/SNMP_AgentsServer/SNMPAgentsServer.hpp \
#    ../../../testApps/ccACU/apiModules/UI_ServerClass/UIServerClass.hpp \
#    ../../../testApps/ccACU/deviceModules/APT_WMM/APT_WMM_Class.hpp \
#    ../../../testApps/ccACU/deviceModules/ManagedSwitch/ManagedSwitchClass.hpp \
#    ../../../testApps/ccACU/deviceModules/Modem/ModemClass.hpp \
#    ../../../testApps/ccACU/deviceModules/TPM/TPM_Class.hpp \
#    ../../../testApps/ccACU/deviceModules/TxRx/TxRx_Class.hpp \
#    ../../../testApps/ccACU/osApp_Serialization.hpp \
#    ../../../testApps/ccACU/osApp_Solution.hpp \
#    Application_Platform_Main.hpp




#SOURCES += \
#    ../../../../ccLibs/devices/adafruit_ft232h/adafruit_ft232h.cpp \
#    ../../../../ccNOos/ccLibs/acs/satComControl.cpp \
#    ../../../../ccNOos/ccLibs/devices/ADRF6650/ADRF6650.cpp \
#    ../../../../ccNOos/ccLibs/devices/HMC1119/HMC1119.cpp \
#    ../../../../ccNOos/ccLibs/devices/HMR3300/HMR3300.cpp \
#    ../../../../ccNOos/ccLibs/devices/LTC2360/LTC2360.cpp \
#    ../../../../ccNOos/ccLibs/devices/MCP23017/MCP23017.cpp \
#    ../../../../ccNOos/ccLibs/devices/NEO_LEA_M8T/NEO_LEA_M8T.cpp \
#    ../../../../ccNOos/ccLibs/mcs/motionControl.cpp \
#    ../../../../ccNOos/computeModule/compute_module_class.cpp \
#    ../../../../ccNOos/consoleMenu/console_menu_class.cpp \
#    ../../../../ccNOos/executionSystem/execution_system_class.cpp \
#    ../../../../ccNOos/ioDevice/io_device_class.cpp \
#    ../../../../ccNOos/tests/testApps/SatComACS/Application_Solution.cpp \
#    ../../../../ccNOos/tests/testApps/SatComACS/PlatformApp_Serialization.cpp \
#    ../../../../ccNOos/tests/testApps/SatComACS/apiModules/ConsoleMenu/MenuAPI.cpp \
#    ../../../../ccNOos/tests/testApps/SatComACS/apiModules/LCDKeyPad/LCDapi.cpp \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/APT_WMM/APTmodule.cpp \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/TPM/TPMmodule.cpp \
#    ../../../../ccNOos/tests/testApps/SatComACS/deviceModules/TxRx/TxRxmodule.cpp \
#    ../../../../exeThread/exe_thread.cpp \
#    ../../../../osExecutionSystem/os_execution_system.cpp \
#    ../../../../serialComms/serial_comms_class.cpp \
#    ../../../../stdIOExtProc/stdio_extproc.cpp \
#    ../../../../tcpComms/tcp_comms_class.cpp \
#    ../../../testApps/ccACU/apiModules/CGI_ServerClass/CGIServerClass.cpp \
#    ../../../testApps/ccACU/apiModules/SNMP_AgentsServer/SNMPAgentsServer.cpp \
#    ../../../testApps/ccACU/apiModules/UI_ServerClass/UIServerClass.cpp \
#    ../../../testApps/ccACU/deviceModules/APT_WMM/APT_WMM_Class.cpp \
#    ../../../testApps/ccACU/deviceModules/ManagedSwitch/ManagedSwitchClass.cpp \
#    ../../../testApps/ccACU/deviceModules/Modem/ModemClass.cpp \
#    ../../../testApps/ccACU/deviceModules/TPM/TPM_Class.cpp \
#    ../../../testApps/ccACU/deviceModules/TxRx/TxRx_Class.cpp \
#    ../../../testApps/ccACU/osApp_Serialization.cpp \
#    ../../../testApps/ccACU/osApp_Solution.cpp \
#    Application_Platform_Main.cpp




ccNOosDIR = $$absolute_path($${PWD}/../../../../ccNOos/)
ccOSDIR = $$absolute_path($${PWD}/../../../../)

SatComACSappDIR = $${ccNOosDIR}/tests/testApps/SatComACS
ccNOosPlatformDIR = $${ccNOosDIR}/tests/testPlatforms
ccACUappDIR = $${ccOSDIR}/tests/testApps/ccACU
ccOSMainsDIR = $${ccOSDIR}/tests/testMainFiles/ccACU/QtCreator_Cpp

############################################################ LIBs
# Select All Libs
CONFIG += ccNOosAllLibs
# Select All Devices
CONFIG += ccNOosAllDevs
CONFIG += ccOSDevs_ft232h

include($${ccACUappDIR}/ccACU.pri)

INCLUDEPATH += $$ccOSMainsDIR
INCLUDEPATH += $$ccNOosPlatformDIR

HEADERS += $$ccNOosPlatformDIR/Platform_ccOS.hpp
HEADERS += $$ccOSMainsDIR/Application_Platform_Main.hpp

SOURCES += $$ccOSMainsDIR/Application_Platform_Main.cpp


message("Includes:")
for(msg, INCLUDEPATH) {
    message($$msg)
}
message("Headers:")
for(msg, HEADERS) {
    message($$msg)
}
message("Sources:")
for(msg, SOURCES) {
    message($$msg)
}
message("Objects:")
for(msg, OBJECTS) {
    message($$msg)
}
message("Source Extensions:")
for(msg, QMAKE_EXT_CPP) {
    message($$msg)
}
message("Header Extensions:")
for(msg, QMAKE_EXT_H) {
    message($$msg)
}


