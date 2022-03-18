#ifndef __APT_WMM__
#define __APT_WMM__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "APTmodule.h"
// extend to the exe thread of ccOS from compute module of ccNOos
#include "exe_thread.hpp"

class nbserial_class;

// Exe Thread - APT WMM Device Module
class APT_WMM_Class : public exe_thread_class
{
private:
	struct aptStruct* APT;
	struct wmmStruct* WMM;
	nbserial_class* gpsSerialPort;
	nbserial_class* eCompassSerialPort;
	struct computeModuleStruct compModData;
	char* InstanceIDstring;
public:
	static int InstanceCount;
	char* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	APT_WMM_Class(struct aptStruct* APTin,
		struct wmmStruct* WMMin,
		nbserial_class* gpsSerialPortin,
		nbserial_class* eCompassSerialPortin
	);

};


#endif // !__APT_WMM__
