#ifndef __MGDSWITCH__
#define __MGDSWITCH__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "PlatformApp_Serialization.h"
#include "exe_thread.hpp"
// include ccOS comm modules and libraries


// Data Structure - Managed Switch Device Module
struct ManagedSwitchStruct
{
	struct computeModuleStruct compModData = CreateComputeModuleStruct();
};




// Exe Thread - Managed Switch Device Module
class ManagedSwitchClass : public exe_thread_class
{
protected:
	char* InstanceIDstring;
	struct ManagedSwitchStruct* data;
public:
	static int InstanceCount;
	char* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	ManagedSwitchClass(struct ManagedSwitchStruct* dataIn);
};










#endif // !__MGDSWITCH__
