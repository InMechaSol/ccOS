#ifndef __MGDSWITCH__
#define __MGDSWITCH__

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


// Data Structure - Managed Switch Device Module
struct ManagedSwitchStruct
{
	struct computeModuleStruct compModData;
};




// Exe Thread - Managed Switch Device Module
class ManagedSwitchClass : public exe_thread_class
{
protected:
	std::string InstanceIDstring;
	struct ManagedSwitchStruct* data;
public:
	static int InstanceCount;
	std::string* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	ManagedSwitchClass(struct ManagedSwitchStruct* dataIn);
};










#endif // !__MGDSWITCH__
