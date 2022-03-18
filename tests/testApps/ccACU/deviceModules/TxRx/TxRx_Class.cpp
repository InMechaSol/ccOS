#include "TxRx_Class.hpp"


char* TxRx_Class::IDString()
{
	return InstanceIDstring;
}
int TxRx_Class::InstanceCount = 0;
TxRx_Class::TxRx_Class(	struct txRxStruct* datain,
						SPI_DeviceClass* SPIin
					) :	exe_thread_class(&compModData)
{
	compModData = CreateComputeModuleStruct();
	InstanceCount++;
	InstanceIDstring = (char*)"TxRx_00";
}
int TxRx_Class::mod_setup() { return RETURN_ERROR; }
int TxRx_Class::mod_loop() { return RETURN_ERROR; }
void TxRx_Class::mod_systick() {}
int TxRx_Class::mod_excphandler() { return RETURN_ERROR; }
