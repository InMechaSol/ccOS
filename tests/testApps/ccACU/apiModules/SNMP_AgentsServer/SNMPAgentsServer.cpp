#include "SNMPAgentsServer.hpp"
#include "osApp_Solution.hpp"


char* SNMP_AgentsAPIServer::IDString()
{
	return InstanceIDstring;
}
int SNMP_AgentsAPIServer::InstanceCount = 0;
SNMP_AgentsAPIServer::SNMP_AgentsAPIServer(struct SNMPServerStruct* dataIn, ccACU_Class* ccACUPtrIn) : exe_thread_class(&dataIn->compModData)
{
	InstanceCount++;
	InstanceIDstring = (char*)"UIServer_00";
	ccACUPtr = ccACUPtrIn;
	data = dataIn;
}
int SNMP_AgentsAPIServer::mod_setup() { return RETURN_ERROR; }
int SNMP_AgentsAPIServer::mod_loop() { return RETURN_ERROR; }
void SNMP_AgentsAPIServer::mod_systick() {}
int SNMP_AgentsAPIServer::mod_excphandler() { return RETURN_ERROR; }