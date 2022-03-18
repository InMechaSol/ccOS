#include "ModemClass.hpp"



char* ModemClass::IDString()
{
	return InstanceIDstring;
}
int ModemClass::InstanceCount = 0;
ModemClass::ModemClass(struct ModemStruct* dataIn) : exe_thread_class(&dataIn->compModData)
{
	InstanceCount++;
	InstanceIDstring = (char*)"Modem_00";
}
int ModemClass::mod_setup() { return RETURN_ERROR; }
int ModemClass::mod_loop() { return RETURN_ERROR; }
void ModemClass::mod_systick() {}
int ModemClass::mod_excphandler() { return RETURN_ERROR; }
