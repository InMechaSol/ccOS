#ifndef __TPMCLASS__
#define __TPMCLASS__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "PlatformApp_Serialization.h"
#include "exe_thread.hpp"
// include ccOS comm modules and libraries

// Exe Thread - TPM Device Module
class TPM_Class : public exe_thread_class
{
private:
	struct tpmStruct* TPM;
	SPI_DeviceClass* downConverterSPI;
	SPI_DeviceClass* powerMeterSPI;
	struct computeModuleStruct compModData;
	char* InstanceIDstring;
public:
	static int InstanceCount;
	char* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	TPM_Class(	struct tpmStruct* TPMin,
				SPI_DeviceClass* downConverterSPIin,
				SPI_DeviceClass* powerMeterSPIin
				);

};




#endif // !__TPMCLASS__
