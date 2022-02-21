#ifndef __SERIALCOMMSH__
#define __SERIALCOMMSH__

#include "io_device.h"
#ifdef __cplusplus
#include <cstring>
#else
#include <string.h>
#endif

#ifdef _WIN32
#include <windows.h>
#else

#endif

struct portParametersStruct
{
#ifdef _WIN32
    const char* portName;
    HANDLE hComm;
#else
    std::string portName;
    int hComm;
#endif
    struct SerialDeviceStruct serialdev;
};
struct portParametersStruct buildportParametersStruct(const char* portName, int baudRate);


UI_8 isComPortOpen(struct portParametersStruct* paramsPtr);
void openComPort(struct portParametersStruct* paramsPtr);
void closeComPort(struct portParametersStruct* paramsPtr);
int readComString(struct portParametersStruct* paramsPtr);
int readComLine(struct portParametersStruct* paramsPtr);
int writeComString(struct portParametersStruct* paramsPtr);

#ifdef __cplusplus

class nbserial_class :public Serial_DeviceClass
{
protected:
	struct portParametersStruct* parmsPtr = nullptr;
	bool readLine = true;		// indicates readdevice will read a line vs read a full buffer
	int opendevice();
	int closedevice();
	int readdevice();
	int writedevice();
	UI_8 isdeviceopen();
public:
	nbserial_class(struct portParametersStruct* parmsPtrIn);
	void setPortParameters(struct portParametersStruct params);
	struct portParametersStruct* getPortParameters();
};

#endif


#endif // !__SERIALCOMMSH__
