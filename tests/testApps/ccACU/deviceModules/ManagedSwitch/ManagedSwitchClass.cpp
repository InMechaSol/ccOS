#include "ManagedSwitchClass.hpp"



std::string* ManagedSwitchClass::IDString()
{
	return &InstanceIDstring;
}
int ManagedSwitchClass::InstanceCount = 0;
ManagedSwitchClass::ManagedSwitchClass(struct ManagedSwitchStruct* dataIn) : exe_thread_class(&dataIn->compModData)
{
	InstanceCount++;
	InstanceIDstring = "ManagedSwitch_00";
}
int ManagedSwitchClass::mod_setup() { return RETURN_ERROR; }
int ManagedSwitchClass::mod_loop() { return RETURN_ERROR; }
void ManagedSwitchClass::mod_systick() {}
int ManagedSwitchClass::mod_excphandler() { return RETURN_ERROR; }
