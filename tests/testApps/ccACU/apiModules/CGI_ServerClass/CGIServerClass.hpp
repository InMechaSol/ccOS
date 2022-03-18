#ifndef __CGISERVER__
#define __CGISERVER__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "PlatformApp_Serialization.h"
#include "exe_thread.hpp"
// include ccOS comm modules and libraries




class ccACU_Class;


// Data Structure - CGI API Module
struct CGIServerStruct
{
	struct computeModuleStruct compModData = CreateComputeModuleStruct();

};

// Exe Thread - CGI API Module
class CGI_ServerClass : public exe_thread_class
{
protected:
	char* InstanceIDstring;
	struct CGIServerStruct* data;
	ccACU_Class* ccACUPtr;
public:
	static int InstanceCount;
	char* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	CGI_ServerClass(struct CGIServerStruct* dataIn, ccACU_Class* ccACUPtrIn);
};

#endif // !__CGISERVER__
