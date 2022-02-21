#include "serial_comms.c"


/////////////////////////////////////////////////////////////////////////////
// Common cross-platform C++ Wrappers
/////////////////////////////////////////////////////////////////////////////

nbserial_class::nbserial_class(struct portParametersStruct* parmsPtrIn) :Serial_DeviceClass(&parmsPtrIn->serialdev)
{
	parmsPtr = parmsPtrIn;
	// devdata buffer pointers are nullptr still...
}
void nbserial_class::setPortParameters(struct portParametersStruct params)
{
	*parmsPtr = params;
}
struct portParametersStruct* nbserial_class::getPortParameters()
{
	return parmsPtr;
}



UI_8 nbserial_class::isdeviceopen()
{
	return isComPortOpen(parmsPtr);
}
int nbserial_class::opendevice()
{
	openComPort(parmsPtr);
	if (isdeviceopen())
		return RETURN_SUCCESS;
	else
		return RETURN_ERROR;
}
int nbserial_class::closedevice()
{
	closeComPort(parmsPtr);
	return RETURN_SUCCESS;
}
int nbserial_class::readdevice()
{
	if (readLine)
		return readComLine(parmsPtr);
	else
		return readComString(parmsPtr);
}
int nbserial_class::writedevice()
{
	return writeComString(parmsPtr);
}

