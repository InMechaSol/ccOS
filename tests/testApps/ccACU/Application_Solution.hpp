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

enum ModemTypes
{
	ModemTypes_None,
	ModemTypes_iDirect_950mp,
	ModemTypes_EFData_
};
struct ModemStruct
{
	enum ModemTypes type = ModemTypes_None;
};

struct ManagedSwitchStruct
{

};

struct ccACUStruct : public SatComACSStruct
{
	struct ModemStruct ModemData;
	struct ManagedSwitchStruct SwitchData;
};

class ManagedSwitchClass : public exe_thread_class
{
protected:
	struct ManagedSwitchStruct* data;
public:
	ManagedSwitchClass(struct ManagedSwitchStruct* dataIn);
};

class ModemClass : public exe_thread_class
{
protected:
	struct ModemStruct* data;
public:
	ModemClass(struct ModemStruct* dataIn);
};

// ccACU_Class inherits from the SatComACS Compute Module
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

	class ModemClass* ModemPtr = nullptr;
	class ManagedSwitchClass* ManagedSwitchPtr = nullptr;

public:
	ccACU_Class(struct ccACUStruct* dataIn);

};


#endif // !__APP_SOLUTION__
