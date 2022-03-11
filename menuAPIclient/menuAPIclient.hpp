#ifndef __MENUAPICLIENT__
#define __MENUAPICLIENT__

#include "exe_thread.hpp"


class menuAPIClientclass : public exe_thread_class
{
public:
	struct computeModuleStruct compModData = CreateComputeModuleStruct();
	char* keysinbuffer = nullptr;
	menuAPIClientclass(char* keysinbufferin);
	virtual int ReadFromDevice() = 0;
	virtual void EchoToUI(struct devicedatastruct* devptr);
	virtual void ReadfromUI(struct devicedatastruct* devptr);
	virtual void SendToDevice() = 0;	
    void mod_systick();
    int mod_excphandler();
};

class TCPmenuAPIClientclass : public menuAPIClientclass
{
public:
	TCPmenuAPIClientclass();
	TCPmenuAPIClientclass(char* connect2ipaddr, uint16_t connect2port, char* keysinbufferin);
	int ReadFromDevice();
	void SendToDevice();
	int mod_setup();
	int mod_loop();
	char* IDString();
};

class UDPmenuAPIClientclass : public menuAPIClientclass
{
public:
	int ReadFromDevice();
	void SendToDevice();
	int mod_setup();
	int mod_loop();
	char* IDString();
};

#endif //!__MENUAPICLIENT__