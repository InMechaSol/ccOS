#ifndef __APT_WMM__
#define __APT_WMM__

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

// Exe Thread - APT WMM Device Module
class APT_WMM_Class : public exe_thread_class
{
private:
	struct aptStruct* APT;
	struct wmmStruct* WMM;
	nbserial_class* gpsSerialPort;
	nbserial_class* eCompassSerialPort;
	struct computeModuleStruct compModData;
	std::string InstanceIDstring;
public:
	static int InstanceCount;
	std::string* IDString();
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
