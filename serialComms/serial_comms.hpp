#ifndef __SERIAL_COMMS__
#define __SERIAL_COMMS__

#include "version_config.hpp"

#ifdef _WIN32
#include <windows.h>
#include <codecvt>
#else

#endif


#include <iostream>
#include <cstring>



struct portParametersStruct
{
#ifdef _WIN32
	std::string portName = "COM1";
	HANDLE hComm = INVALID_HANDLE_VALUE;
#else
	std::string portName = "/dev/tty1";
	int hComm = -1;
#endif
	struct SerialDeviceStruct serialdev;
	std::string* outString = nullptr;
	std::string* inString = nullptr;
};
struct portParametersStruct buildportParametersStruct(const char* portName, int baudRate, std::string* inStringPtr, std::string* outStringPtr);

bool isComPortOpen(struct portParametersStruct* paramsPtr);
void openComPort(struct portParametersStruct* paramsPtr);
void closeComPort(struct portParametersStruct* paramsPtr);
int readComString(struct portParametersStruct* paramsPtr);
int writeComString(struct portParametersStruct* paramsPtr);

class nbserial_class :public Serial_DeviceClass
{
protected:
	struct portParametersStruct* parmsPtr = nullptr;
	int opendevice();
	int closedevice();
	int readdevice();
	int writedevice();
	UI_8 isdeviceopen();
public:
	nbserial_class(struct portParametersStruct* parmsPtrIn);
	void setPortParameters(struct portParametersStruct params);
	struct portParametersStruct getPortParameters();
};


#endif // !__SERIAL_COMMS__

