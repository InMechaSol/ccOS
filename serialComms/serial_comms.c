#include "serial_comms.h"

struct portParametersStruct buildportParametersStruct(const char* portName, int baudRate)
{
    struct portParametersStruct outstruct;
    outstruct.portName = (char*)portName;
#ifdef _WIN32
    outstruct.hComm = INVALID_HANDLE_VALUE;
#else
    outstruct.hComm = -1;
#endif

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

int readComPort(struct portParametersStruct* paramsPtr)
{
    DWORD bytesRead = 0;
    if(paramsPtr->serialdev.readIndex > -1 && paramsPtr->serialdev.readIndex < charBuffMax && paramsPtr->serialdev.devdata.numbytes2Read < charBuffMax)
    {
        if (ReadFile(   paramsPtr->hComm,
                        &(paramsPtr->serialdev.devdata.inbuff.charbuff[paramsPtr->serialdev.readIndex]),
                        paramsPtr->serialdev.devdata.numbytes2Read,
                        &bytesRead,
                        NULL)
            )
        {
            return (int)bytesRead;
        }
        else
            return 0;
    }
    else
        return 0;
}

int writeComPort(struct portParametersStruct* paramsPtr)
{
    if (isComPortOpen(paramsPtr))
    {
        ;
    }
    return 0;
}
#else
UI_8 isComPortOpen(struct portParametersStruct* paramsPtr)
{
    if(paramsPtr->hComm>0)
        return ui8TRUE;
    else
        return ui8FALSE;
}
void openComPort(struct portParametersStruct* paramsPtr)
{
    paramsPtr->hComm = open(paramsPtr->portName, O_RDWR | O_NDELAY);

    // Set parameters
    struct termios tty;

    // Get current settings
    if(tcgetattr(paramsPtr->hComm, &tty) != 0) {
        return;
    }

    tty.c_cflag &= ~PARENB; //disable parity
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; //8 bits
    tty.c_cflag &= ~CRTSCTS; //no hardware flow control
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON; // disable canonical mode (newline terminated input)
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ISIG; // Disable signal chars

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 0;    // Do not wait for any characters
    tty.c_cc[VMIN] = 0;

    // Update rate.
    if (paramsPtr->serialdev.baudRate <= 1200)
        cfsetspeed(&tty, B1200);
    else if(paramsPtr->serialdev.baudRate  <= 2400)
        cfsetspeed(&tty, B2400);
    else if (paramsPtr->serialdev.baudRate  <= 4800)
        cfsetspeed(&tty, B4800);
    else if (paramsPtr->serialdev.baudRate  <= 9600)
        cfsetspeed(&tty, B9600);
    else if (paramsPtr->serialdev.baudRate  <= 19200)
        cfsetspeed(&tty, B19200);
    else if (paramsPtr->serialdev.baudRate  <= 38400)
        cfsetspeed(&tty, B38400);
    else if (paramsPtr->serialdev.baudRate  <= 57600)
        cfsetspeed(&tty, B57600);
    else if (paramsPtr->serialdev.baudRate  <= 115200)
        cfsetspeed(&tty, B115200);

    // Save tty settings
    if (tcsetattr(paramsPtr->hComm, TCSANOW, &tty) != 0)
        return;
}
void closeComPort(struct portParametersStruct* paramsPtr)
{
    close(paramsPtr->hComm);
    paramsPtr->hComm = -1;
}
int readComPort(struct portParametersStruct* paramsPtr)
{
    return 0;
}
int writeComPort(struct portParametersStruct* paramsPtr)
{
    return 0;
}
#endif

