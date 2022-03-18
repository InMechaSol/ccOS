#include "TPM_Class.hpp"

char* TPM_Class::IDString()
{
	return InstanceIDstring;
}
int TPM_Class::InstanceCount = 0;
TPM_Class::TPM_Class(	struct tpmStruct* TPMin,
						SPI_DeviceClass* downConverterSPIin,
						SPI_DeviceClass* powerMeterSPIin
					) : exe_thread_class(&compModData)
{
	compModData = CreateComputeModuleStruct();
	InstanceCount++;
	InstanceIDstring = (char*)"TPM_00";
}
int TPM_Class::mod_setup() { return RETURN_ERROR; }
int TPM_Class::mod_loop() { return RETURN_ERROR; }
void TPM_Class::mod_systick() {}
int TPM_Class::mod_excphandler() { return RETURN_ERROR; }

