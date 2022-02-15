#include "Application_Platform_Main.hpp"

ModemClass::ModemClass(struct ModemStruct* dataIn) : exe_thread_class(&dataIn->compModData)
{

}

ManagedSwitchClass::ManagedSwitchClass(struct ManagedSwitchStruct* dataIn) : exe_thread_class(&dataIn->compModData)
{

}

APT_WMM_Class::APT_WMM_Class(	struct aptStruct* APTin,
								struct wmmStruct* WMMin,
								nbserial_class* gpsSerialPortin,
								nbserial_class* eCompassSerialPortin
							):exe_thread_class(&compModData)
{
	compModData = CreateComputeModuleStruct();
}



TPM_Class::TPM_Class(	struct tpmStruct* TPMin,
						SPI_DeviceClass* downConverterSPIin,
						SPI_DeviceClass* powerMeterSPIin
					) : exe_thread_class(&compModData)
{
	compModData = CreateComputeModuleStruct();
}


TxRx_Class::TxRx_Class(	struct txRxStruct* datain,
						SPI_DeviceClass* SPIin
					) :	exe_thread_class(&compModData)
{
	compModData = CreateComputeModuleStruct();
}


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

}
