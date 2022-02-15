/** \file WIN32_Console.cpp
*   \brief WIN32 Console Implementation of ccNOos_Tests in c++

   Copyright 2021 InMechaSol, Inc

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

Notes:

*/

// This file should only ever be included by one file, PlatformApp_Serialization.cpp
// Therefore, it is not #ifdef guarded and will cause "multiple definition" errors if included more than once.
#include "PlatformApp_Serialization.hpp"


// 4) Basic ability for user console input
void GetMenuChars(char* inStringPtr)
{
    std::cin >> inStringPtr;
}
// 5) Basic ability for user console output
void WriteMenuLine(char* outStringPtr)
{
    std::cout << outStringPtr;
}


// 6) (Optional) Logging Output
void WriteLogLine(char* outStringPtr)
{
    int logLineLen = -1;
    while (logLineLen < charBuffMax)
        if (outStringPtr[++logLineLen] == 0x00)
            break;
    if (logLineLen > 0)
        LogFile.write(outStringPtr, logLineLen);
}
// 7) (Optional) Config Input
void ReadConfigLine(char* inStringPtr)
{
    int confLineLen = 0;
    while (confLineLen < charBuffMax)
    {
        if (0 < configFile.peek())
            configFile.read(&inStringPtr[confLineLen++], 1);
        else
            break;
    }

}


// 9) Global Execution System Instance
//executionSystemClass PLATFORM_EXESYS_NAME(Plat)(uSEC_PER_CLOCK);
// 10) ExeSys API Functions (From Template?)
OSexecutionSystemClass theExecutionSystem(uSEC_PER_CLOCK);
UI_32 getuSecTicks()
{
return theExecutionSystem.getExeDataPtr()->uSecTicks;
}
UI_32 getHourTicks()
{
return theExecutionSystem.getExeDataPtr()->hourTicks;
}
UI_32 getuSecPerSysTick()
{
return theExecutionSystem.getExeDataPtr()->uSecPerSysTick;
}

#include "adafruit_ft232h.hpp"

 
// Instantiate Platform Specific IO Devices
struct adaFruitFT232hstruct PowerMeterADC_dev;
adaFruitUSB_SPI_class PowerMeter_ADC(&PowerMeterADC_dev);

struct adaFruitFT232hstruct PowerMeterPLL_dev;
adaFruitUSB_SPI_class PowerMeter_PLL(&PowerMeterPLL_dev);

struct adaFruitFT232hstruct TxRxSPI_dev;
adaFruitUSB_SPI_class TxRxSPI(&TxRxSPI_dev);

struct portParametersStruct APTGPS_dev;
nbserial_class APT_GPS(&APTGPS_dev);

struct portParametersStruct APTeComp_dev;
nbserial_class APT_eCompass(&APTeComp_dev);

///////////////////////////////////////////////////////////////////////
// Platform and Application Specific IO Device Functions
// - These are called cyclic by the SatComACS Module Loop Function
// - They need only check if asynchronous io operation has completed
//
void writeAttenuatorValues(struct txRxStruct* txRxStructPtrIn) 
{ 
    TxRxSPI.TriggerWriteOperation();
}
UI_8 readGPS(struct gpsStruct* gpsStructPtrIn) 
{ 
    if (APT_GPS.NewDataReadIn())
    {
        // set to Parse from data string to gps struct
        gpsStructPtrIn->devptr = APT_GPS.GetDevPtr();
        return ui8TRUE;
        // clear flag - done in satcomacs module
        //APT_GPS.ClearNewDataReadInFlag();
    }
    else
        return ui8FALSE;
}
UI_8 readEcompass(struct eCompStruct* eCompStructPtrIn) 
{ 
    if (APT_eCompass.NewDataReadIn())
    {
        // set to Parse from data string to gps struct
        eCompStructPtrIn->devptr = APT_eCompass.GetDevPtr();
        return ui8TRUE;
        // clear flag - done in satcomacs module
        //APT_eCompass.ClearNewDataReadInFlag();
    }
    else
        return ui8FALSE;
}
void readFreqConv(struct freqConvStruct* freqConvStructPtrIn) 
{ 
    if (PowerMeter_PLL.NewDataReadIn())
    {
        // Parse from data string to gps struct

        // clear flag
        PowerMeter_PLL.ClearNewDataReadInFlag();
    }
}
void writeFreqConv(struct freqConvStruct* freqConvStructPtrIn) 
{ 
    PowerMeter_PLL.TriggerWriteOperation();
}
void readPowerMeter(struct powerMeterStruct* powerMeterStructPtrIn) 
{ 
    if (PowerMeter_ADC.NewDataReadIn())
    {
        // Parse from data string to gps struct

        // clear flag
        PowerMeter_ADC.ClearNewDataReadInFlag();
    }
}
void writePowerMeter(struct powerMeterStruct* powerMeterStructPtrIn) 
{ 
    PowerMeter_ADC.TriggerWriteOperation();
}

class ccACU_ApplicationClass
{
public:
    // Linked Entry Points for execution of module entry points
    linkedEntryPointClass setupListHead;
    linkedEntryPointClass loopListHead;
    linkedEntryPointClass systickListHead;
    linkedEntryPointClass exceptionListHead;

    // The ccACU Compute Module and Data
    ccACU_Class ccACU_compMod;
    struct ccACUStruct ccACU_data;

    // API Compute Modules
    UI_ServerClass UIServer_exeThread;
    SNMP_AgentsAPIServer SNMPServer_exeThread;
    CGI_ServerClass CGIServer_exeThread;

    // Device Compute Modules from ccNOos / SatComACS layer
    APT_WMM_Class APT_WMM_exeThread;
    TPM_Class TPM_exeThread;

    // Device Compute Modules from ccOS / ccACU layer
    ModemClass Modem_exeThread;
    struct ModemStruct Modem_data;
    ManagedSwitchClass Switch_exeThread;
    struct ManagedSwitchStruct Switch_data;

    // pointer to The static instance of Execution system for ccACU application
    OSexecutionSystemClass* theExecutionSystemPtr;

    // Construction of the Application
    ccACU_ApplicationClass(OSexecutionSystemClass* theExecutionSystemPtrIn) :
        // link the ccACU compute module to its ccACU data instance
        ccACU_compMod(&ccACU_data),

        // link the api compute modules to the ccACU compute module
        UIServer_exeThread(&ccACU_compMod),
        SNMPServer_exeThread(&ccACU_compMod),
        CGIServer_exeThread(&ccACU_compMod),

        // link the device compute modules and the data objects on which they operate
        APT_WMM_exeThread(&ccACU_data.APT, &ccACU_data.WMM, &APT_GPS, &APT_eCompass),
        TPM_exeThread(&ccACU_data.TPM, &PowerMeter_PLL, &PowerMeter_ADC),

        Modem_exeThread(&Modem_data),
        Switch_exeThread(&Switch_data),

        // create ccNOos exeSys linking for
        // - main process execution of linked compute modules
        // - systick execution of linked compute modules
        // - exception handling execution of linked compute modules
        setupListHead(&ccACU_compMod, nullptr),
        loopListHead(&ccACU_compMod, nullptr),
        systickListHead(nullptr, nullptr),
        exceptionListHead(&ccACU_compMod, nullptr)
    {
        // link the execution system instance
        theExecutionSystemPtr = theExecutionSystemPtrIn;
        // link the execution system module lists
        theExecutionSystemPtr->LinkTheListsHead(
            &setupListHead,
            &loopListHead,
            &systickListHead,
            &exceptionListHead
        );
    }

};
//ccOS_APP_CLASS




