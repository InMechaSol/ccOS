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
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#endif

struct portParametersStruct
{
    const char* portName;
#ifdef _WIN32
    HANDLE hComm;
#else
    int hComm;
#endif
    struct SerialDeviceStruct serialdev;
};
struct portParametersStruct buildportParametersStruct(const char* portName, int baudRate);


UI_8 isComPortOpen(struct portParametersStruct* paramsPtr);
void openComPort(struct portParametersStruct* paramsPtr);
void closeComPort(struct portParametersStruct* paramsPtr);
int readComPort(struct portParametersStruct* paramsPtr);
int writeComPort(struct portParametersStruct* paramsPtr);

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
