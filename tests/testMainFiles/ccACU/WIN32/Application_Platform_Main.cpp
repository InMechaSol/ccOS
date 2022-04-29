#include "Application_Platform_Main.hpp"
#include "Platform_ccOS.hpp"

ccACU_ApplicationClass theApplicationExample(&theExecutionSystem);


// this is a satcom acs only function, called in the setup function
void linkAPIioDevices(struct SatComACSStruct* satcomacsStructPtrIn)
{
    theApplicationExample.linkAPIioDevices();

    // APT Module IO Devices are linked at the ReadGPS and ReadeComp Functions
    /*GPSPortParams = buildportParametersStruct("\\\\.\\COM32", 9600);
    openComPort(&GPSPortParams);
    eCompPortParams = buildportParametersStruct("\\\\.\\COM30", 19200);
    openComPort(&eCompPortParams);*/

    // Menu API Devices are linked here
    //LCDKeyPadPortParams = buildportParametersStruct("\\\\.\\COM9", 9600);
    //openComPort(&LCDKeyPadPortParams);
    /*satcomacsStructPtrIn->LCDKeyPad.devptr = &LCDKeyPadPortParams.serialdev.devdata;
    satcomacsStructPtrIn->LCDKeyPad.devptr->triggerWriteOperation = ui8TRUE;*/


    //ConsoleMenuTCPServerStruct = createTCPServerStruct();
    //ConsoleMenuTCPStructTemp = createTCPStruct();
    //ConsoleMenuTCPServerStruct.listenPort = 55500;
    //OpenTCPServer(&ConsoleMenuTCPServerStruct);
    /*satcomacsStructPtrIn->ConsoleMenu.devptr = &ConsoleMenuTCPServerStruct.tcpData.devdata;
    satcomacsStructPtrIn->ConsoleMenu.showHelp = ui8TRUE;
    satcomacsStructPtrIn->ConsoleMenu.devptr->triggerWriteOperation = ui8TRUE;*/

    //ConsoleMenuTCPServerStruct2 = createTCPServerStruct();
    //ConsoleMenuTCPStructTemp2 = createTCPStruct();
    //ConsoleMenuTCPServerStruct2.listenPort = 55600;
    //OpenTCPServer(&ConsoleMenuTCPServerStruct2);

    // Config and Log API Devices are linked here
    ; // currently linked to std I/O via writeLogLine and readConfigLine Functions


}

/////////////////////////////////////////////////////////////////////////////////
// SatComACS functions, called cyclically in main loop for each api module

void GetMenuChars(struct uiStruct* uiStructPtrin)
{
    ;// theApplicationExample.GetMenuChars();
}
// 5) Basic ability for user console output
void WriteMenuLine(struct uiStruct* uiStructPtrin)
{
    ;
    //// if Consolue Menu
    //if (uiStructPtrin->devptr == &ConsoleMenuTCPServerStruct.tcpData.devdata)
    //{
    //    // based on state
    //    // write to tcp socket

    //    if (uiStructPtrin->clearScreen) {
    //        ConsoleMenuTCPStructTemp.devdata.numbytes2Write = ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write;
    //        stringInit(ConsoleMenuTCPStructTemp.devdata.outbuff.charbuff, ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff);
    //        stringInit(ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff, terminalClearString());
    //        ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write = stringLength(terminalClearString());
    //        TCPWrite(&ConsoleMenuTCPServerStruct.tcpData);
    //        stringInit(ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff, ConsoleMenuTCPStructTemp.devdata.outbuff.charbuff);
    //        ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write = ConsoleMenuTCPStructTemp.devdata.numbytes2Write;
    //        uiStructPtrin->clearScreen = ui8FALSE;
    //    }
    //    if (ConsoleMenu_tcpServer.WriteDev() > 0);
    //    if (uiStructPtrin->showPrompt) {
    //        ConsoleMenuTCPStructTemp.devdata.numbytes2Write = ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write;
    //        stringInit(ConsoleMenuTCPStructTemp.devdata.outbuff.charbuff, ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff);
    //        stringInit(ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff, terminalPromptString(uiStructPtrin->currentUserLevel));
    //        ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write = stringLength(terminalPromptString(uiStructPtrin->currentUserLevel));
    //        TCPWrite(&ConsoleMenuTCPServerStruct.tcpData);
    //        stringInit(ConsoleMenu_tcpServer.GetDevPtr()->outbuff.charbuff, ConsoleMenuTCPStructTemp.devdata.outbuff.charbuff);
    //        ConsoleMenu_tcpServer.GetDevPtr()->numbytes2Write = ConsoleMenuTCPStructTemp.devdata.numbytes2Write;
    //        uiStructPtrin->showPrompt = ui8FALSE;
    //    }
    //}
    //// if LCD KeyPad
    //else if (uiStructPtrin->devptr == &LCDKeyPadPortParams.serialdev.devdata)
    //{
    //    ;
    //}

}


// 6) (Optional) Logging Output
void WriteLogLine(struct logStruct* logStructPtrin)
{
    // Send log to stdout
    std::cout << &logStructPtrin->devptr->outbuff.charbuff[0];
}
// 7) (Optional) Config Input
void ReadConfigLine(struct configStruct* configStructPtrin)
{
    // Allow for configuration via stdin
    if (stdInThreadRunning == ui8FALSE)
    {
        if (configStructPtrin->devptr->triggerWriteOperation == ui8FALSE)
        {
            configStructPtrin->devptr->newDataReadIn = ui8TRUE;
            //configStructPtrin->parseIndex = 0;
            stdInThreadRunning = ui8TRUE;
        }
    }
}


UI_8 readGPS(struct gpsStruct* gpsStructPtrIn)
{
    return theApplicationExample.readGPS();
}
UI_8 readEcompass(struct eCompStruct* eCompStructPtrIn)
{
    return theApplicationExample.readEcompass();
}


#define SETPIN_TX_C16(high_low)    
#define SETPIN_TX_C8(high_low)     
#define SETPIN_TX_C4(high_low)     
#define SETPIN_TX_C2(high_low)     
#define SETPIN_TX_C1(high_low)     
#define SETPIN_TX_C0_25(high_low)  
#define SETPIN_TX_C0_50(high_low)  
#define SETPIN_TX_LE(high_low)     

#define SETPIN_RX_C16(high_low)    
#define SETPIN_RX_C8(high_low)     
#define SETPIN_RX_C4(high_low)     
#define SETPIN_RX_C2(high_low)     
#define SETPIN_RX_C1(high_low)     
#define SETPIN_RX_C0_25(high_low)  
#define SETPIN_RX_C0_50(high_low)  
#define SETPIN_RX_LE(high_low)     

#define SETPIN_XX_C16(high_low)    
#define SETPIN_XX_C8(high_low)     
#define SETPIN_XX_C4(high_low)     
#define SETPIN_XX_C2(high_low)     
#define SETPIN_XX_C1(high_low)     
#define SETPIN_XX_C0_25(high_low)  
#define SETPIN_XX_C0_50(high_low)  
#define SETPIN_XX_LE(high_low)     

#define SETPIN_MDM1_SW(high_low)   
#define SETPIN_MDM2_SW(high_low)   
#define SETPIN_MDM3_SW(high_low)   
#define SETPIN_MDM4_SW(high_low)   

SatComACS_TxRx_WriteFuncsTemplate(std::this_thread::sleep_for(std::chrono::milliseconds(1)))
void readFreqConv(struct freqConvStruct* freqConvStructPtrIn) { ; }
void writeFreqConv(struct freqConvStruct* freqConvStructPtrIn) { ; }
void readPowerMeter(struct powerMeterStruct* powerMeterStructPtrIn) { ; }
void writePowerMeter(struct powerMeterStruct* powerMeterStructPtrIn) { ; }

ccOSVersionsTemplate
int main(int argc, char** argv)
{
	// Start the systick thread
	std::thread systickThread(&OSexecutionSystemClass::ExecuteSysTick, std::ref(theExecutionSystem));

	// Start the exe_thread modules
	theApplicationExample.LinkAndStartExeThreads();

	// run setup
	theExecutionSystem.ExecuteSetup();
	for (;;)
	{
		// run loop
		theExecutionSystem.ExecuteLoop();
	}
	return RETURN_ERROR; 
}