#ifndef __SNMPSERVER__
#define __SNMPSERVER__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "PlatformApp_Serialization.h"
// include ccOS execution system and exe thread module
//#include "os_execution_system.hpp"
#include "exe_thread.hpp"
// include ccOS comm modules and libraries
//#include "serial_comms.hpp"
//#include "udp_comms.hpp"
//#include "tcp_comms.hpp"
//#include "stdio_extproc.hpp"

class ccACU_Class;

// Data Structure - SNMP API Module
struct SNMPServerStruct
{
	struct computeModuleStruct compModData = CreateComputeModuleStruct();

};

// Exe Thread - SNMP API Module
class SNMP_AgentsAPIServer : public exe_thread_class
{
protected:
	char* InstanceIDstring;
	struct SNMPServerStruct* data;
	ccACU_Class* ccACUPtr;
public:
	static int InstanceCount;
	char* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	SNMP_AgentsAPIServer(struct SNMPServerStruct* dataIn, ccACU_Class* ccACUPtrIn);
};

#endif // !__SNMPSERVER__