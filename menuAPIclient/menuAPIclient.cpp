
#include "menuAPIclient.hpp"
#include "console_menu.h"
#include <iostream>
#include <tcp_comms.h>




std::thread stdInThread;
bool stdInThreadRunning = false;
bool runONCE = true; // to launch std::in thread once
void readStdIn(char* inStringPtr)
{
	do {
		if (stdInThreadRunning == true)
		{
			std::cin >> inStringPtr;
			stdInThreadRunning = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	} while (true);
}



void menuAPIClientclass::EchoToUI(struct devicedatastruct* devptr)
{
	// echo out from the inbuff of the device servicing the menu api
	std::cout << &devptr->inbuff.charbuff[0] << std::flush;
}
void menuAPIClientclass::ReadfromUI(struct devicedatastruct* devptr)
{
	// read into the outbuff of the device servicing the menu api
	if (runONCE)
	{
		stdInThread = std::thread(readStdIn, &devptr->outbuff.charbuff[0]);
		runONCE = false;
		devptr->numbytes2Write = 0;
		//devptr->newDataReadIn = ui8FALSE;
		stdInThreadRunning = true;
	}
	else if (stdInThreadRunning == false)
	{
		devptr->numbytes2Write = stringLength((const char*)&devptr->outbuff.charbuff[0]);
		//devptr->newDataReadIn = ui8TRUE;
		stdInThreadRunning = true;
	}
}
menuAPIClientclass::menuAPIClientclass(char* keysinbufferin):exe_thread_class(&compModData)
{
	keysinbuffer = keysinbufferin;
    mainLoopMS = 100;
	
}
void menuAPIClientclass::mod_systick() { ; }
int menuAPIClientclass::mod_excphandler() { return ui8FALSE; }

struct tcpStruct tcpData = createTCPStruct();
tcp_client_class theTCPClient(&tcpData);
TCPmenuAPIClientclass::TCPmenuAPIClientclass(char* connect2ipaddr, uint16_t connect2port, char* keysinbufferin):menuAPIClientclass(keysinbufferin)
{
	tcpData.ipaddrstring = connect2ipaddr;
	tcpData.serverPortNum = connect2port;
}
int TCPmenuAPIClientclass::ReadFromDevice() 
{
	return theTCPClient.ReadDev();
}
void TCPmenuAPIClientclass::SendToDevice() 
{
	theTCPClient.WriteDev();
}
int TCPmenuAPIClientclass::mod_setup() 
{ 
	OpenTCPClient(&tcpData);	
	setMainLoopMS(100);
	enableMainLoop();
	return ui8FALSE; 
}
int TCPmenuAPIClientclass::mod_loop()
{
	static enum tcpServerClientStatus lastStatus = tcpstat_uninitialized;
	char lastChar = 0x00;
	static bool need2sendUL = false;
	theTCPClient.ShutdownRestartClientSocket();	
	do {
		ReadFromDevice(); lastChar = 0x00;
		if (tcpData.devdata.numbytesReadIn > 0)
		{			
			if (tcpData.devdata.numbytesReadIn < charBuffMax)
			{
				tcpData.devdata.inbuff.charbuff[tcpData.devdata.numbytesReadIn] = 0x00;
			}
			else
			{
				lastChar = tcpData.devdata.inbuff.charbuff[tcpData.devdata.numbytesReadIn - 1];
				tcpData.devdata.inbuff.charbuff[tcpData.devdata.numbytesReadIn-1] = 0x00;				
				if (lastChar != 0x00)
				{
					EchoToUI(&tcpData.devdata);
					tcpData.devdata.inbuff.charbuff[0] = lastChar;
					tcpData.devdata.inbuff.charbuff[1] = 0x00;					
				}
			}			
			EchoToUI(&tcpData.devdata);
			
		}
	} while (tcpData.devdata.numbytesReadIn > 0 );
	
	// if just connected, send userlevel command
	if (tcpData.tcpStatus!= lastStatus)
	{
		lastStatus = tcpData.tcpStatus;
		if (lastStatus == tcpstat_connected)
		{		
			need2sendUL = true;
			if (need2sendUL)
			{
				tcpData.devdata.numbytes2Write = SN_PrintF(&tcpData.devdata.outbuff.charbuff[0], charBuffMax, "UserLevel:%s;", UserLevelString.c_str());
				need2sendUL = false;
			}
		}
	}
	ReadfromUI(&tcpData.devdata);
	SendToDevice();
	return ui8FALSE; 
}
char* TCPmenuAPIClientclass::IDString()
{
	return (char*)"TCPClient";
}


int UDPmenuAPIClientclass::ReadFromDevice() { return 0; }
void UDPmenuAPIClientclass::SendToDevice() {}
int UDPmenuAPIClientclass::mod_setup() { return ui8FALSE; }
int UDPmenuAPIClientclass::mod_loop() { return ui8FALSE; }
char* UDPmenuAPIClientclass::IDString()
{
	return (char*)"UDPClient";
}
