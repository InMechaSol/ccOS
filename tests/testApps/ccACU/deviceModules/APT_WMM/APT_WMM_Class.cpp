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
	gpsSerialPort = gpsSerialPortin;
	eCompassSerialPort = eCompassSerialPortin;
	WMM = WMMin;
}
int APT_WMM_Class::mod_setup() 
{ 
	return RETURN_ERROR; 
}
int APT_WMM_Class::mod_loop() 
{
	static std::string lastGPSPortName = "";
	static std::string eCompGPSPortName = "";

	if (lastGPSPortName != gpsSerialPort->getPortParameters()->portName)
	{
		gpsSerialPort->CloseDev();
		lastGPSPortName = gpsSerialPort->getPortParameters()->portName;
	}

	// if port not open, try to open
	if (!gpsSerialPort->IsDevOpen())
	{
		try
		{
			gpsSerialPort->OpenDev();
		}
		catch (...)
		{
			;// closePort();
		}
	}
	// if port open, try to read
	else
	{
		try
		{
			while (readComString(gpsSerialPort->getPortParameters()) > 0)
			{
				this->APT->GPS.newGPSData = ui8TRUE;
			}
		}
		catch (...)
		{
			APT->GPS.Connected = false;
			gpsSerialPort->CloseDev();
		}

	}

	return RETURN_ERROR; 
}
void APT_WMM_Class::mod_systick() {}
int APT_WMM_Class::mod_excphandler() { return RETURN_ERROR; }
