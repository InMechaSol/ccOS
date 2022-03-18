#ifndef __TXRXMOD__
#define __TXRXMOD__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "PlatformApp_Serialization.h"
#include "exe_thread.hpp"
// include ccOS comm modules and libraries


// Exe Thread - TxRx Module
class TxRx_Class : public exe_thread_class
{
private:
	struct txRxStruct* data;
	SPI_DeviceClass* SPI;
	struct computeModuleStruct compModData;
	char* InstanceIDstring;
public:
	static int InstanceCount;
	char* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	TxRx_Class(	struct txRxStruct* datain,
				SPI_DeviceClass* SPIin
				);
};



#endif // !__TXRXMOD__
