## ccACU depends on following directories
if(!defined(ccOSDIR,var)) {
    error("ccOSDIR is not defined")
}
if(!defined(SatComACSappDIR,var)) {
    error("SatComACSappDIR is not defined")
}
if(!defined(ccACUappDIR,var)) {
    error("ccACUappDIR is not defined")
}

## ccACU depends on the ccOS project
include($${ccOSDIR}/ccOS.pri)
## ccACU depends on the SatComACS project
include($${SatComACSappDIR}/satComACS.pri)


INCLUDEPATH += $$ccACUappDIR

INCLUDEPATH += $$ccACUappDIR/apiModules/CGI_ServerClass
INCLUDEPATH += $$ccACUappDIR/apiModules/SNMP_AgentsServer
INCLUDEPATH += $$ccACUappDIR/apiModules/UI_ServerClass
INCLUDEPATH += $$ccACUappDIR/deviceModules/APT_WMM
INCLUDEPATH += $$ccACUappDIR/deviceModules/TPM
INCLUDEPATH += $$ccACUappDIR/deviceModules/TxRx
INCLUDEPATH += $$ccACUappDIR/deviceModules/Modem
INCLUDEPATH += $$ccACUappDIR/deviceModules/ManagedSwitch

HEADERS += $$ccACUappDIR/osApp_Solution.hpp
HEADERS += $$ccACUappDIR/osApp_Serialization.hpp
HEADERS += $$ccACUappDIR/apiModules/CGI_ServerClass/CGIServerClass.hpp
HEADERS += $$ccACUappDIR/apiModules/SNMP_AgentsServer/SNMPAgentsServer.hpp
HEADERS += $$ccACUappDIR/apiModules/UI_ServerClass/UIServerClass.hpp
HEADERS += $$ccACUappDIR/deviceModules/APT_WMM/APT_WMM_Class.hpp
HEADERS += $$ccACUappDIR/deviceModules/TPM/TPM_Class.hpp
HEADERS += $$ccACUappDIR/deviceModules/TxRx/TxRx_Class.hpp
HEADERS += $$ccACUappDIR/deviceModules/Modem/ModemClass.hpp
HEADERS += $$ccACUappDIR/deviceModules/ManagedSwitch/ManagedSwitchClass.hpp

SOURCES += $$ccACUappDIR/osApp_Solution.cpp
SOURCES += $$ccACUappDIR/osApp_Serialization.cpp
SOURCES += $$ccACUappDIR/apiModules/CGI_ServerClass/CGIServerClass.cpp
SOURCES += $$ccACUappDIR/apiModules/SNMP_AgentsServer/SNMPAgentsServer.cpp
SOURCES += $$ccACUappDIR/apiModules/UI_ServerClass/UIServerClass.cpp
SOURCES += $$ccACUappDIR/deviceModules/APT_WMM/APT_WMM_Class.cpp
SOURCES += $$ccACUappDIR/deviceModules/TPM/TPM_Class.cpp
SOURCES += $$ccACUappDIR/deviceModules/TxRx/TxRx_Class.cpp
SOURCES += $$ccACUappDIR/deviceModules/Modem/ModemClass.cpp
SOURCES += $$ccACUappDIR/deviceModules/ManagedSwitch/ManagedSwitchClass.cpp




