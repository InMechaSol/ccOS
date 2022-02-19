#ifndef __SERIALCOMMSH__
#define __SERIALCOMMSH__

#include "io_device.h"
#include <string.h>

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
    std::string portName = "/dev/tty1";
    int hComm = -1;
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


#endif // !__SERIALCOMMSH__
