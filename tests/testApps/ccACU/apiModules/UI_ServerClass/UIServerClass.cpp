#include "UIServerClass.hpp"
#include "osApp_Solution.hpp"


std::string* UI_ServerClass::IDString()
{
	return &InstanceIDstring;
}
int UI_ServerClass::InstanceCount = 0;
UI_ServerClass::UI_ServerClass(struct UIServerStruct* dataIn, ccACU_Class* ccACUPtrIn) : exe_thread_class(&dataIn->compModData)
{
	InstanceCount++;
	InstanceIDstring = "UIServer_00";
	ccACUPtr = ccACUPtrIn;
	data = dataIn;
}
int UI_ServerClass::mod_setup() { return RETURN_ERROR; }
int UI_ServerClass::mod_loop() { return RETURN_ERROR; }
void UI_ServerClass::mod_systick() {  }
int UI_ServerClass::mod_excphandler() { return RETURN_ERROR; }
