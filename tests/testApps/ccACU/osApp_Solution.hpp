#ifndef __APP_SOLUTION__
#define __APP_SOLUTION__

#include "application_solution.h"
#include "os_execution_system.hpp"

// Include all Device Modules
#include "APT_WMM_Class.hpp"
#include "ManagedSwitchClass.hpp"
#include "ModemClass.hpp"
#include "TxRx_Class.hpp"
#include "TPM_Class.hpp"

// Forward Declare all API Modules
class UI_ServerClass;
class SNMP_AgentsAPIServer;
class CGI_ServerClass;

// Top Level ccACU Data Structure
struct ccACUStruct
{
	struct ModemStruct ModemData;
	struct ManagedSwitchStruct SwitchData;
	struct uiStruct ConsoleMenu2 = createuiStruct();
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
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
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
	UI_ServerClass* getUIServerPtr();

};


#endif // !__APP_SOLUTION__
