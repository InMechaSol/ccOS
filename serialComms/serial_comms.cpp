#include "nbserial_class.h"

#ifdef _WIN32
/////////////////////////////////////////////////////////////////////////////
// Plain C Windows
/////////////////////////////////////////////////////////////////////////////
bool isComPortOpen(struct portParametersStruct* paramsPtr)
{
	return paramsPtr->hComm != INVALID_HANDLE_VALUE;
}
void openComPort(struct portParametersStruct* paramsPtr)
{
	if (paramsPtr->portName.find("COM") >= 0)
	{
		std::wstring portName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(paramsPtr->portName);

		paramsPtr->hComm = CreateFile(portName.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			0,
			OPEN_EXISTING,
			0,
			0);

		if (!isComPortOpen(paramsPtr))
			throw "Failed to open com port";

		COMMTIMEOUTS lpCommTimeouts;

		if (GetCommTimeouts(paramsPtr->hComm, &lpCommTimeouts))
		{
			lpCommTimeouts.ReadIntervalTimeout = MAXDWORD;
			lpCommTimeouts.ReadTotalTimeoutConstant = 0;
			lpCommTimeouts.ReadTotalTimeoutMultiplier = 0;

			if (!SetCommTimeouts(paramsPtr->hComm, &lpCommTimeouts))
				throw "Failed to set com port timeouts";
		}

		DCB dcb;

		FillMemory(&dcb, sizeof(dcb), 0);
		if (!GetCommState(paramsPtr->hComm, &dcb))     // get current DCB
			throw "Failed to get current com parameters";

		// Update DCB rate.
		if (paramsPtr->baudRate <= 1200)
			dcb.BaudRate = CBR_1200;
		else if (paramsPtr->baudRate <= 2400)
			dcb.BaudRate = CBR_2400;
		else if (paramsPtr->baudRate <= 4800)
			dcb.BaudRate = CBR_4800;
		else if (paramsPtr->baudRate <= 9600)
			dcb.BaudRate = CBR_9600;
		else if (paramsPtr->baudRate <= 14400)
			dcb.BaudRate = CBR_14400;
		else if (paramsPtr->baudRate <= 19200)
			dcb.BaudRate = CBR_19200;
		else if (paramsPtr->baudRate <= 38400)
			dcb.BaudRate = CBR_38400;
		else if (paramsPtr->baudRate <= 57600)
			dcb.BaudRate = CBR_57600;
		else if (paramsPtr->baudRate <= 115200)
			dcb.BaudRate = CBR_115200;

		dcb.fBinary = true;

		if (paramsPtr->parity == parity_none)
		{
			dcb.fParity = false;
			dcb.Parity = 0;
		}
		else
			throw "Parity not yet implemented";

		if (paramsPtr->handshake == handshake_none)
		{
			dcb.fOutxCtsFlow = false;
			dcb.fOutxDsrFlow = false;
			dcb.fOutX = false;
			dcb.fDtrControl = DTR_CONTROL_DISABLE;
			dcb.fDsrSensitivity = false;
			dcb.fInX = false;
			dcb.fNull = false;
			dcb.fRtsControl = RTS_CONTROL_DISABLE;
			dcb.fAbortOnError = false;
		}
		else
			throw "Flow control not yet implemented";


		if (paramsPtr->stopBits == stop_one)
		{
			dcb.StopBits = ONESTOPBIT;
		}
		else
			throw "Only one stop bit currently implemented";


		// Set new state.
		if (!SetCommState(paramsPtr->hComm, &dcb))
			throw "Failed to set updated com parameters";
	}
}
void closeComPort(struct portParametersStruct* paramsPtr)
{
	CloseHandle(paramsPtr->hComm);
	paramsPtr->hComm = INVALID_HANDLE_VALUE;
}
int readComString(struct portParametersStruct* paramsPtr)
{
	char inbuff[1] = { 0x00 };
	DWORD bytesRead = 0;
	int totalBytesRead = 0;
	if (isComPortOpen(paramsPtr))
	{
		(*paramsPtr->inString) = "";
		do
		{
			if (ReadFile(paramsPtr->hComm, inbuff, 1, &bytesRead, NULL))
			{
				if (bytesRead == 1)
				{
					(*paramsPtr->inString) += inbuff[0];
					totalBytesRead++;
					if (inbuff[0] == '\0' || inbuff[0] == '\n' || inbuff[0] == '\r')
						return totalBytesRead;
				}
				else
				{
					return totalBytesRead;
				}
			}
			else
#ifdef __cplusplus
			{
				throw "failed read operation, comm port";
				return RETURN_ERROR;
			}

#else
				return RETURN_ERROR;
#endif
		} while (bytesRead == 1);
		return 0;
	}
	else
		return 0;
}
int writeComString(struct portParametersStruct* paramsPtr)
{
	return RETURN_ERROR;
}
#else
/////////////////////////////////////////////////////////////////////////////
// Plain C Linux
/////////////////////////////////////////////////////////////////////////////
bool isComPortOpen(struct portParametersStruct* paramsPtr)
{
	return (parmsPtr->hCom > 0);
}
void openComPort(struct portParametersStruct* paramsPtr)
{
	if (parmsPtr->portName.find("tty") >= 0)
	{
		if (paramsPtr->parity != parity_none)
			throw "Parity not yet implemented";

		if (paramsPtr->handshake != handshake_none)
			throw "Flow control not yet implemented";


		if (paramsPtr->stopBits != stop_one)
			throw "Only one stop bit currently implemented";

		paramsPtr->hCom = open(paramsPtr->portName.c_str(), O_RDWR | O_NDELAY);

		if (!isPortOpen())
			throw "Failed to open ";//<< paramsPtr->portName.c_str() << " | "<< paramsPtr->hCom << "\n";

		// Set parameters
		struct termios tty;

		// Get current settings
		if (tcgetattr(paramsPtr->hCom, &tty) != 0) {
			throw "Failed to get current tty parameters";
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
		tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

		tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
		tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

		tty.c_cc[VTIME] = 0;    // Do not wait for any characters
		tty.c_cc[VMIN] = 0;

		// Update rate.
		if (paramsPtr->baudRate <= 1200)
			cfsetspeed(&tty, B1200);
		else if (paramsPtr->baudRate <= 2400)
			cfsetspeed(&tty, B2400);
		else if (paramsPtr->baudRate <= 4800)
			cfsetspeed(&tty, B4800);
		else if (paramsPtr->baudRate <= 9600)
			cfsetspeed(&tty, B9600);
		else if (paramsPtr->baudRate <= 19200)
			cfsetspeed(&tty, B19200);
		else if (paramsPtr->baudRate <= 38400)
			cfsetspeed(&tty, B38400);
		else if (paramsPtr->baudRate <= 57600)
			cfsetspeed(&tty, B57600);
		else if (paramsPtr->baudRate <= 115200)
			cfsetspeed(&tty, B115200);

		// Save tty settings
		if (tcsetattr(paramsPtr->hCom, TCSANOW, &tty) != 0)
			throw "Failed to set updated com parameters";

	}
}
void closeComPort(struct portParametersStruct* paramsPtr)
{
	close(paramsPtr->hCom);
	paramsPtr->hCom = -1;
}
int readComString(struct portParametersStruct* paramsPtr)
{
	char inbuff[1] = { 0x00 };
	int bytesRead = 0;
	int totalBytesRead = 0;
	if (isPortOpen())
	{
		(*paramsPtr->inString) = "";
		do
		{
			bytesRead = read(paramsPtr->hCom, inbuff, 1);
			if (bytesRead > -1)
			{
				if (bytesRead == 1)
				{
					(*inString) += inbuff[0];
					totalBytesRead++;
					if (inbuff[0] == '\0' || inbuff[0] == '\n' || inbuff[0] == '\r')
						return totalBytesRead;
				}
				else
				{
					return totalBytesRead;
				}
			}
			else
				throw "failed read operation, comm port";

		} while (bytesRead == 1);

	}
	else
		return 0;
}
int writeComString(struct portParametersStruct* paramsPtr)
{
	return RETURN_ERROR;
}

#endif
/////////////////////////////////////////////////////////////////////////////
// Common cross-platform C
/////////////////////////////////////////////////////////////////////////////
struct portParametersStruct buildportParametersStruct(const char* portName, int baudRate, std::string* inStringPtr, std::string* outStringPtr)
{
	struct portParametersStruct outstruct;
	outstruct.portName = portName;
	outstruct.baudRate = baudRate;
	outstruct.inString = inStringPtr;
	outstruct.outString = outStringPtr;
}

/////////////////////////////////////////////////////////////////////////////
// Common cross-platform C++ Wrappers
/////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
nbserial_class::nbserial_class(struct portParametersStruct* parmsPtrIn) :iodevice_class(&parmsPtrIn->devdata)
{
	parmsPtr = parmsPtrIn;
	// devdata buffer pointers are nullptr still...
}
void nbserial_class::setPortParameters(struct portParametersStruct params)
{
	*parmsPtr = params;
}
struct portParametersStruct nbserial_class::getPortParameters()
{
	struct portParametersStruct outStruct = *parmsPtr;
	return outStruct;
}



bool nbserial_class::isdeviceopen()
{
	return isComPortOpen(parmsPtr);
}
int nbserial_class::opendevice()
{
	openComPort(parmsPtr);
	if (isdeviceopen())
		return RETURN_SUCCESS;
	else
		return RETURN_ERROR;
}
int nbserial_class::closedevice()
{
	closeComPort(parmsPtr);
	return RETURN_SUCCESS;
}
int nbserial_class::readdevice()
{
	return readComString(parmsPtr);
}
int nbserial_class::writedevice()
{
	return writeComString(parmsPtr);
}
#endif