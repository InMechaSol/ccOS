#ifndef __TXRXMOD__
#define __TXRXMOD__

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


// Exe Thread - TxRx Module
class TxRx_Class : public exe_thread_class
{
private:
	struct txRxStruct* data;
	SPI_DeviceClass* SPI;
	struct computeModuleStruct compModData;
	std::string InstanceIDstring;
public:
	static int InstanceCount;
	std::string* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	TxRx_Class(	struct txRxStruct* datain,
				SPI_DeviceClass* SPIin
				);
};



#endif // !__TXRXMOD__
