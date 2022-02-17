#include "APT_WMM_Class.hpp"


std::string* APT_WMM_Class::IDString()
{
	return &InstanceIDstring;
}
int APT_WMM_Class::InstanceCount = 0;
APT_WMM_Class::APT_WMM_Class(struct aptStruct* APTin,
	struct wmmStruct* WMMin,
	nbserial_class* gpsSerialPortin,
	nbserial_class* eCompassSerialPortin
) :exe_thread_class(&compModData)
{
	compModData = CreateComputeModuleStruct();
	InstanceCount++;
	InstanceIDstring = "APT_WMM_00";
}
int APT_WMM_Class::mod_setup() { return RETURN_ERROR; }
int APT_WMM_Class::mod_loop() { return RETURN_ERROR; }
void APT_WMM_Class::mod_systick() {}
int APT_WMM_Class::mod_excphandler() { return RETURN_ERROR; }
