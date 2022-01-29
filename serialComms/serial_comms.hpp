#ifndef __NBSERIAL_CLASS__
#define __NBSERIAL_CLASS__

#include "iodevice_class.h"

#ifdef _WIN32
#include <windows.h>
#include <codecvt>
#else

#endif


#include <iostream>
#ifdef __cplusplus
#include <cstring>
extern "C" {
#else
#include <string>
#endif

enum portStopBitsEnum
{
	stop_none = 0,
	stop_one = 1,
	stop_two = 2,
	stop_onepointfive = 3
};
enum portParityEnum
{
	parity_none = 0,
	parity_even = 1,
	parity_odd = 2,
	parity_mark = 3,
	parity_space = 4
};
enum portHandshakeEnum
{
	handshake_none = 0,
	handshake_requesttosend = 1,
	handshake_requesttosendxonxoff = 2,
	handshake_xonxoff = 3
};

struct portParametersStruct
{
#ifdef _WIN32
	std::string portName = "COM1";
	HANDLE hComm = INVALID_HANDLE_VALUE;
#else
	std::string portName = "/dev/tty1";
	int hComm = -1;
#endif
	int baudRate = 9600;
	int dataBits = 8;
	enum portStopBitsEnum stopBits = stop_one;
	enum portParityEnum parity = parity_none;
	enum portHandshakeEnum handshake = handshake_none;
	struct devicedatastruct devdata;
	std::string* outString = nullptr;
	std::string* inString = nullptr;
};
struct portParametersStruct buildportParametersStruct(const char* portName, int baudRate, std::string* inStringPtr, std::string* outStringPtr);

bool isComPortOpen(struct portParametersStruct* paramsPtr);
void openComPort(struct portParametersStruct* paramsPtr);
void closeComPort(struct portParametersStruct* paramsPtr);
int readComString(struct portParametersStruct* paramsPtr);
int writeComString(struct portParametersStruct* paramsPtr);


#ifdef __cplusplus
}
class nbserial_class :public iodevice_class
{
protected:
	struct portParametersStruct* parmsPtr = nullptr;
	int opendevice();
	int closedevice();
	int readdevice();
	int writedevice();
	bool isdeviceopen();
public:
	nbserial_class(struct portParametersStruct* parmsPtrIn);
	void setPortParameters(struct portParametersStruct params);
	struct portParametersStruct getPortParameters();
};

#endif // !__cplusplus
#endif // !__NBSERIAL_CLASS__

