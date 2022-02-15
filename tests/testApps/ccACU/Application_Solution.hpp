#ifndef __APP_SOLUTION__
#define __APP_SOLUTION__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "PlatformApp_Serialization.h"
// include ccOS execution system and exe thread module
#include "os_execution_system.hpp"
#include "exe_thread.hpp"
// include ccOS comm modules and libraries
#include "serial_comms.hpp"
#include "udp_comms.hpp"
#include "tcp_comms.hpp"
#include "stdio_extproc.hpp"

class UI_ServerClass;
class SNMP_AgentsAPIServer;
class CGI_ServerClass;


// Data Structure - Modem Device Module
enum ModemTypes
{
	ModemTypes_None,
	ModemTypes_iDirect_950mp,
	ModemTypes_EFData_
};
struct ModemStruct
{
	struct computeModuleStruct compModData;
	enum ModemTypes type = ModemTypes_None;
};

// Data Structure - Managed Switch Device Module
struct ManagedSwitchStruct
{
	struct computeModuleStruct compModData;
};

// Top Level ccACU Data Structure
struct ccACUStruct
{
	struct ModemStruct ModemData;
	struct ManagedSwitchStruct SwitchData;
};



// Exe Thread - Managed Switch Device Module
class ManagedSwitchClass : public exe_thread_class
{
protected:
	struct ManagedSwitchStruct* data;
public:
	ManagedSwitchClass(struct ManagedSwitchStruct* dataIn);
};

// Exe Thread - Modem Device Module
class ModemClass : public exe_thread_class
{
protected:
	struct ModemStruct* data;
public:
	ModemClass(struct ModemStruct* dataIn);
};

// Exe Thread - APT WMM Device Module
class APT_WMM_Class : public exe_thread_class
{
private:
	struct aptStruct* APT;
	struct wmmStruct* WMM;
	nbserial_class* gpsSerialPort;
	nbserial_class* eCompassSerialPort;
	struct computeModuleStruct compModData;
public:
	APT_WMM_Class(	struct aptStruct* APTin, 
					struct wmmStruct* WMMin,
					nbserial_class* gpsSerialPortin,
					nbserial_class* eCompassSerialPortin
					);

};

// Exe Thread - TPM Device Module
class TPM_Class : public exe_thread_class
{
private:
	struct tpmStruct* TPM;
	SPI_DeviceClass* downConverterSPI;
	SPI_DeviceClass* powerMeterSPI;
	struct computeModuleStruct compModData;
public:
	TPM_Class(	struct tpmStruct* TPMin,
				SPI_DeviceClass* downConverterSPIin,
				SPI_DeviceClass* powerMeterSPIin
				);

};

// Exe Thread - TxRx Module
class TxRx_Class : public exe_thread_class
{
private:
	struct txRxStruct* data;
	SPI_DeviceClass* SPI;
	struct computeModuleStruct compModData;
public:
	TxRx_Class(	struct txRxStruct* datain,
				SPI_DeviceClass* SPIin
				);
};

// ccACU_Class inherits from the SatComACS Compute Module
// - and adds ccOS level exe_threads for devices
class ccACU_Class : public SatComACS_class
{
private:
	friend class UI_ServerClass;
	friend class SNMP_AgentsAPIServer;
	friend class CGI_ServerClass;

protected:
	struct ccACUStruct* data;

	UI_ServerClass* UIServerPtr = nullptr;
	SNMP_AgentsAPIServer* SNMPServerPtr = nullptr;
	CGI_ServerClass* CGIServerPtr = nullptr;

	APT_WMM_Class* APT_WMMPtr = nullptr;
	TPM_Class* TPMPtr = nullptr;
	TxRx_Class* TxRxPtr = nullptr;
	ModemClass* ModemPtr = nullptr;
	ManagedSwitchClass* ManagedSwitchPtr = nullptr;

public:
	ccACU_Class(	struct ccACUStruct* dataIn,
		UI_ServerClass* UIServerPtrin,
		SNMP_AgentsAPIServer* SNMPServerPtrin,
		CGI_ServerClass* CGIServerPtrin,
		APT_WMM_Class* APT_WMMPtrin,
		TPM_Class* TPMPtrin,
		TxRx_Class* TxRxPtrin,
		ModemClass* ModemPtrin,
		ManagedSwitchClass* ManagedSwitchPtrin
				);

};


#endif // !__APP_SOLUTION__
