#ifndef __CGISERVER__
#define __CGISERVER__

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

// Data Structure - CGI API Module
struct CGIServerStruct
{
	struct computeModuleStruct compModData;
};

// Exe Thread - CGI API Module
class CGI_ServerClass : public exe_thread_class
{
protected:
	std::string InstanceIDstring;
	struct CGIServerStruct* data;
	ccACU_Class* ccACUPtr;
public:
	static int InstanceCount;
	std::string* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	CGI_ServerClass(struct CGIServerStruct* dataIn, ccACU_Class* ccACUPtrIn);
};

#endif // !__CGISERVER__
