#ifndef __TPMCLASS__
#define __TPMCLASS__

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

// Exe Thread - TPM Device Module
class TPM_Class : public exe_thread_class
{
private:
	struct tpmStruct* TPM;
	SPI_DeviceClass* downConverterSPI;
	SPI_DeviceClass* powerMeterSPI;
	struct computeModuleStruct compModData;
	std::string InstanceIDstring;
public:
	static int InstanceCount;
	std::string* IDString();
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
