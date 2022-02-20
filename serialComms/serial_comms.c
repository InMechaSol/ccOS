#include "serial_comms.h"

struct portParametersStruct buildportParametersStruct(const char* portName, int baudRate)
{
    struct portParametersStruct outstruct;
    outstruct.portName = (char*)portName;
    outstruct.hComm = INVALID_HANDLE_VALUE;
    outstruct.serialdev.baudRate = baudRate;
    outstruct.serialdev.dataBits = 8;
    outstruct.serialdev.devdata = createDeviceStruct();
    outstruct.serialdev.handshake = handshake_none;
    outstruct.serialdev.parity = parity_none;
    outstruct.serialdev.stopBits = stop_one;
    return outstruct;
}


#ifdef _WIN32
void openComPort(struct portParametersStruct* paramsPtr)
{
    paramsPtr->hComm = CreateFileA(paramsPtr->portName,
                                   GENERIC_READ | GENERIC_WRITE,
                                   0,
                                   NULL,
                                   OPEN_EXISTING,
                                   0,
                                   NULL);

    DWORD dw;
    if(paramsPtr->hComm == INVALID_HANDLE_VALUE)
    {
        dw = GetLastError();
    }


    if(isComPortOpen(paramsPtr))
    {
        COMMTIMEOUTS lpCommTimeouts;

        if (GetCommTimeouts(paramsPtr->hComm, &lpCommTimeouts))
        {
            lpCommTimeouts.ReadIntervalTimeout = MAXDWORD;
            lpCommTimeouts.ReadTotalTimeoutConstant = 0;
            lpCommTimeouts.ReadTotalTimeoutMultiplier = 0;

            if (!SetCommTimeouts(paramsPtr->hComm, &lpCommTimeouts))
            {
                // close the port
                closeComPort(paramsPtr);
            }
            else
            {
                DCB dcb;

                FillMemory(&dcb, sizeof(dcb), 0);
                if (!GetCommState(paramsPtr->hComm, &dcb))     // get current DCB
                {
                    // close the port
                    closeComPort(paramsPtr);
                }
                else
                {
                    // Update DCB rate.
                    if (paramsPtr->serialdev.baudRate <= 1200)
                        dcb.BaudRate = CBR_1200;
                    else if (paramsPtr->serialdev.baudRate <= 2400)
                        dcb.BaudRate = CBR_2400;
                    else if (paramsPtr->serialdev.baudRate <= 4800)
                        dcb.BaudRate = CBR_4800;
                    else if (paramsPtr->serialdev.baudRate <= 9600)
                        dcb.BaudRate = CBR_9600;
                    else if (paramsPtr->serialdev.baudRate <= 14400)
                        dcb.BaudRate = CBR_14400;
                    else if (paramsPtr->serialdev.baudRate <= 19200)
                        dcb.BaudRate = CBR_19200;
                    else if (paramsPtr->serialdev.baudRate <= 38400)
                        dcb.BaudRate = CBR_38400;
                    else if (paramsPtr->serialdev.baudRate <= 57600)
                        dcb.BaudRate = CBR_57600;
                    else if (paramsPtr->serialdev.baudRate <= 115200)
                        dcb.BaudRate = CBR_115200;
                    else
                    {
                        // close the port
                        closeComPort(paramsPtr);
                        return;
                    }

                    dcb.fBinary = 1;

                    // only supporting parity_none at the moment
                    dcb.fParity = 0;
                    dcb.Parity = 0;

                    // only supporting handshake_none at the moment
                    dcb.fOutxCtsFlow = 0;
                    dcb.fOutxDsrFlow = 0;
                    dcb.fOutX = 0;
                    dcb.fDtrControl = DTR_CONTROL_DISABLE;
                    dcb.fDsrSensitivity = 0;
                    dcb.fInX = 0;
                    dcb.fNull = 0;
                    dcb.fRtsControl = RTS_CONTROL_DISABLE;
                    dcb.fAbortOnError = 0;

                    // only  supporting stop_one at the moment
                    dcb.StopBits = ONESTOPBIT;


                    // Set new state.
                    if (!SetCommState(paramsPtr->hComm, &dcb))
                    {
                        // close the port
                        closeComPort(paramsPtr);
                    }
                }
            }
        }
    }
}
UI_8 isComPortOpen(struct portParametersStruct* paramsPtr)
{
    return (paramsPtr->hComm != INVALID_HANDLE_VALUE);
}

void closeComPort(struct portParametersStruct* paramsPtr)
{
    CloseHandle(paramsPtr->hComm);
    paramsPtr->hComm = INVALID_HANDLE_VALUE;
}
int readComString(struct portParametersStruct* paramsPtr)
{
    DWORD bytesRead = 0;
    int totalBytesRead = 0;
    if (isComPortOpen(paramsPtr))
    {
        do{
            if(charBuffMax-totalBytesRead == 0)
            {
                totalBytesRead = 0;
            }
            if (ReadFile(paramsPtr->hComm, &(paramsPtr->serialdev.devdata.inbuff.charbuff[totalBytesRead]), charBuffMax-totalBytesRead, &bytesRead, NULL))
            {
                totalBytesRead += bytesRead;
            }
        }while(bytesRead>0);
        return totalBytesRead;
    }
    return 0;
}
int readComLine(struct portParametersStruct* paramsPtr)
{
    DWORD bytesRead = 0;
    int totalBytesRead = 0;
    if (isComPortOpen(paramsPtr))
    {
        do{
            if (ReadFile(paramsPtr->hComm, &(paramsPtr->serialdev.devdata.inbuff.charbuff[totalBytesRead]), 1, &bytesRead, NULL))
            {

                if(bytesRead == 1)
                {
                    if (    paramsPtr->serialdev.devdata.inbuff.charbuff[totalBytesRead] == '\n' ||
                            paramsPtr->serialdev.devdata.inbuff.charbuff[totalBytesRead] == '\r')
                    {

                        if(totalBytesRead==0)
                        {
                            paramsPtr->serialdev.devdata.inbuff.charbuff[totalBytesRead] = 0x00;
                            return 0;
                        }
                        else
                        {
                            totalBytesRead++;
                            paramsPtr->serialdev.devdata.inbuff.charbuff[totalBytesRead] = 0x00;
                            return totalBytesRead;
                        }
                    }
                    else
                        totalBytesRead++;
                }
                else
                {
                    return totalBytesRead;
                }
            }
        }while(bytesRead>0);
        return totalBytesRead;
    }
    return 0;
}
int writeComString(struct portParametersStruct* paramsPtr)
{
    if (isComPortOpen(paramsPtr))
    {
        ;
    }
    return 0;
}
#else
UI_8 isComPortOpen(struct portParametersStruct* paramsPtr);
void openComPort(struct portParametersStruct* paramsPtr);
void closeComPort(struct portParametersStruct* paramsPtr);
int readComString(struct portParametersStruct* paramsPtr);
int writeComString(struct portParametersStruct* paramsPtr);
#endif

