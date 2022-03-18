#include "CGIServerClass.hpp"
#include "osApp_Solution.hpp"


char* CGI_ServerClass::IDString()
{
	return InstanceIDstring;
}
int CGI_ServerClass::InstanceCount = 0;
CGI_ServerClass::CGI_ServerClass(struct CGIServerStruct* dataIn, ccACU_Class* ccACUPtrIn) : exe_thread_class(&dataIn->compModData)
{
	InstanceCount++;
	InstanceIDstring = (char*)"CGIServer_00";
	ccACUPtr = ccACUPtrIn;
	data = dataIn;
}
int CGI_ServerClass::mod_setup() { return RETURN_ERROR; }
int CGI_ServerClass::mod_loop() { return RETURN_ERROR; }
void CGI_ServerClass::mod_systick() {  }
int CGI_ServerClass::mod_excphandler() { return RETURN_ERROR; }
