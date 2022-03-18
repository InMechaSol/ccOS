#include "osApp_Solution.hpp"
#include "CGIServerClass.hpp"
#include "UIServerClass.hpp"
#include "SNMPAgentsServer.hpp"


ccACU_Class::ccACU_Class(
	struct ccACUStruct* dataIn,
	UI_ServerClass* UIServerPtrin,
	SNMP_AgentsAPIServer* SNMPServerPtrin,
	CGI_ServerClass* CGIServerPtrin,
	APT_WMM_Class* APT_WMMPtrin,
	TPM_Class* TPMPtrin,
	TxRx_Class* TxRxPtrin,
	ModemClass* ModemPtrin,
	ManagedSwitchClass* ManagedSwitchPtrin
	):SatComACS_class()
{
	data = dataIn;
	UIServerPtr = UIServerPtrin;
	SNMPServerPtr = SNMPServerPtrin;
	CGIServerPtr = CGIServerPtrin;
	APT_WMMPtr = APT_WMMPtrin;
	TPMPtr = TPMPtrin;
	TxRxPtr = TxRxPtrin;
	ModemPtr = ModemPtrin;
	ManagedSwitchPtr = ManagedSwitchPtrin;
}
int ccACU_Class::mod_setup() 
{ 
	// Run the SatComACS setup
	return SatComACS_class::mod_setup(); 
}
int ccACU_Class::mod_loop() { return SatComACS_class::mod_loop(); }
void ccACU_Class::mod_systick() { SatComACS_class::mod_systick(); }
int ccACU_Class::mod_excphandler() { return SatComACS_class::mod_excphandler(); }
UI_ServerClass* ccACU_Class::getUIServerPtr() { return UIServerPtr; }
