#ifndef __MODEMCLASS__
#define __MODEMCLASS__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "PlatformApp_Serialization.h"
// include ccOS execution system and exe thread module
//#include "os_execution_system.hpp"
#include "exe_thread.hpp"
// include ccOS comm modules and libraries
#include "serial_comms.hpp"
//#include "udp_comms.hpp"
//#include "tcp_comms.hpp"
#include "stdio_extproc.hpp"



// Data Structure - Modem Device Module
enum ModemTypes
{
	ModemTypes_None,
	ModemTypes_iDirect_950mp,
	ModemTypes_EFData_
};
struct ModemStruct
{
	struct computeModuleStruct compModData;
	enum ModemTypes type = ModemTypes_None;
};

// Exe Thread - Modem Device Module
class ModemClass : public exe_thread_class
{
protected:
	std::string InstanceIDstring;
	struct ModemStruct* data;
public:
	static int InstanceCount;
	std::string* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	ModemClass(struct ModemStruct* dataIn);
};



#endif // !__MODEMCLASS__
