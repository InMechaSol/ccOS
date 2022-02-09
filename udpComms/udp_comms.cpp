#include "udp_comms.hpp"
#include "os_execution_system.hpp"

const char UDPUIServerClientClass::UI_IP_AddrString[15] = "127.0.0.1";
const unsigned short UDPUIServerClientClass::UI_IP_ClientUI_BindPortArray[2] = { 50501, 50502 };
const char* UDPUIServerClientClass::getUDPUIClientConnectionStateString(enum UDPUIClientConnectionState theCS)
{
    switch (theCS)
    {
    case UDPCommsInit: return "UDPCommsInit";
    case UDPCOmmsDisconnected: return "UDPCommsDisconnected";
    case UDPCommsConnecting: return "UDPCommsConnecting";
    case UDPCommsConnected: return "UDPCommsConnected";
    }
}
const char* UDPUIServerClientClass::getUDPUIClientUserLevelString(enum userlevel theUL)
{
    switch (theUL)
    {
    case usernone: return "usernone";
    case terminaloperator: return "terminaloperator";
    case terminaladmin: return "terminaladmin";
    case developer: return "developer";
    }
}
const char* UDPUIServerClientClass::getUDPUIClientUITypeString(enum uitype theUIT)
{
    switch (theUIT)
    {
    case terminal: return "terminal";
    case largeLCD: return "largeLCD";
    case smallLCD: return "smallLCD";
    }
}

UDP_Class::UDP_Class()
{

}

void UDP_Class::CreateNonBlockingSocket()
{
    if (!socketCreated)
    {
#ifdef _WIN32
        CreateNBSocketWindows();
#else
        CreateNBSocketLinux();
#endif
    }
    socketCreated = true;
}

int UDP_Class::BindSocket(unsigned short bindPort)
{
    int retVal = -1;
    if (!socketBound)
    {
#ifdef _WIN32
        retVal = BindSocketWindows(bindPort);
#else
        retVal = BindSocketLinux(bindPort);
#endif
    }
    if(retVal > 0)
        socketBound = true;
    return retVal;
}

UDP_Class::~UDP_Class()
{
#ifdef _WIN32
    if(socketCreated)
	    closesocket(nbSocket);
	WSACleanup();
#else
    if (socketCreated)
        close(nbSocket);
#endif
}

int UDP_Class::ReceiveFrom(char* buff, int len)
{
    if (socketCreated && socketBound)
    {
#ifdef _WIN32
        return RecieveFromWindows(buff, len);
#else
        return RecieveFromLinux(buff, len);
#endif
    }
    else
        return 0;
	
}

int UDP_Class::ReceiveFrom(char* buff, int len, uint16_t* clientPortParsed)
{
    if (socketCreated && socketBound)
    {
#ifdef _WIN32
        return RecieveFromWindows(buff, len, clientPortParsed);
#else
        return RecieveFromLinux(buff, len, clientPortParsed);
#endif
    }
    else
        return 0;

}

int UDP_Class::SendTo(char* ipaddrstring, int portNum, char* buff, int bytes2send)
{
    if (socketCreated)
    {
#ifdef _WIN32
        return SendToWindows(ipaddrstring, portNum, buff, bytes2send);
#else
        return SendToLinux(ipaddrstring, portNum, buff, bytes2send);
#endif
    }
	return 0;
}



void UDP_Class::CreateNBSocketWindows()
{
#ifdef _WIN32
    unsigned long ul = 1;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
        throw "Error at WSAStartup()\n";

    nbSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (nbSocket == INVALID_SOCKET)
        throw "Error at Socket()\n";

    if (ioctlsocket(nbSocket, FIONBIO, (unsigned long*)&ul) != NO_ERROR)
        throw "Error at ioctlsocket()";
#else
    ;
#endif
}

void UDP_Class::CreateNBSocketLinux()
{
#ifdef _WIN32
    ;
#else
    unsigned long ul = 1;
    

    nbSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (nbSocket == 0)
        throw "Error at Socket()\n";

    int nRet = ioctl(nbSocket, FIONBIO, (unsigned long*)&ul);
    if (nRet != 0)
        throw "Error at ioctlsocket()";
#endif
}



int UDP_Class::BindSocketWindows(unsigned short bindPort)
{
#ifdef _WIN32
    int boundPort = bindPort;

    while (boundPort != 0)
    {
        memset(&sReceiveFromAddr, 0, sizeof(SOCKADDR_IN));
        sReceiveFromAddr.sin_family = AF_INET;
        sReceiveFromAddr.sin_port = htons(boundPort);
        sReceiveFromAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(nbSocket, (SOCKADDR*)&sReceiveFromAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
        {
            int thisErr = WSAGetLastError();
            if (thisErr == WSAEADDRINUSE)
            {
                if (boundPort - bindPort > 1)
                    boundPort = 0;
                else
                    boundPort++;
            }
            else
                throw "Error at bind()";
        }
        else
            return boundPort;
    }
    return boundPort;
    
#else
    return 0;
#endif
}

int UDP_Class::BindSocketLinux(unsigned short bindPort)
{
#ifdef _WIN32
    return 0;
#else
    int boundPort = bindPort;

    while (boundPort != 0)
    {
        memset(&sReceiveFromAddr, 0, sizeof(struct sockaddr_in));
        sReceiveFromAddr.sin_family = AF_INET;
        sReceiveFromAddr.sin_port = htons(boundPort);
        sReceiveFromAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(nbSocket, (struct sockaddr*)&sReceiveFromAddr, sizeof(struct sockaddr_in)) < 0)
        {
            if (errno == EADDRINUSE)
            {
                if (boundPort - bindPort > 1)
                    boundPort = 0;
                else
                    boundPort++;
            }
            else
                throw "Error at bind()";
        }
        else
            return boundPort;
    }
    return boundPort;
    
#endif
}



int UDP_Class::RecieveFromWindows(char* buff, int len)
{
#ifdef _WIN32
    // Get the datagram
    int nBytesRecv = recvfrom(nbSocket, buff, len, 0,
        (SOCKADDR*)&sReceiveFromAddr,
        &nReceiveAddrSize);

    if (nBytesRecv >= 0)
        return nBytesRecv;// printf("Rxd: %d Bytes\n", nBytesRecv);
    else if (WSAGetLastError() == WSAEWOULDBLOCK)
        return 0;// printf("Rxd: 0 Bytes\n");
    else
        throw "Rxd: Error! %d\n", WSAGetLastError();
#else
    return 0;
#endif
    
}

int UDP_Class::RecieveFromLinux(char* buff, int len)
{
#ifdef _WIN32
    return 0;
#else
    // Get the datagram
    int nBytesRecv = recvfrom(nbSocket, buff, len, 0,(struct sockaddr*)&sReceiveFromAddr, &nReceiveAddrSize);
    if (nBytesRecv >= 0)
        return nBytesRecv;//printf("Rxd: %d Bytes\n", nBytesRecv);
    else if (errno == EWOULDBLOCK)
        return 0;//printf("Rxd: 0 Bytes\n");
    else
        throw "Rxd: Error! %d\n", nBytesRecv;
#endif

}

int UDP_Class::RecieveFromWindows(char* buff, int len, uint16_t* clientPortParsed)
{
    int bytesRecieved = RecieveFromWindows(buff, len);
    *clientPortParsed = sReceiveFromAddr.sin_port;
    return bytesRecieved;
}

int UDP_Class::RecieveFromLinux(char* buff, int len, uint16_t* clientPortParsed)
{
    int bytesRecieved = RecieveFromLinux(buff, len);
    *clientPortParsed = sReceiveFromAddr.sin_port;
    return bytesRecieved;
}


int UDP_Class::SendToWindows(char* ipaddrstring, int portNum, char* buff, int bytes2send)
{
#ifdef _WIN32
    
    memset(&sSendToAddr, 0, sizeof(SOCKADDR_IN));
    sSendToAddr.sin_family = AF_INET;
    sSendToAddr.sin_port = htons(portNum);
    sSendToAddr.sin_addr.s_addr = inet_addr(ipaddrstring);

    // send the datagram
    int nBytesSent = sendto(nbSocket, buff, bytes2send, 0, (SOCKADDR*)&sSendToAddr, sizeof(sSendToAddr));
    int wsaerror = WSAGetLastError();
    std::string errorMsg = "Txd: Error! : ";
    if (nBytesSent >= 0)
        return nBytesSent;// printf("Rxd: %d Bytes\n", nBytesRecv);
    else if (wsaerror == WSAEWOULDBLOCK)
        return 0;// printf("Rxd: 0 Bytes\n");
    else
        throw errorMsg + "wsaerror\n";
#else
    return 0;
#endif
}

int UDP_Class::SendToLinux(char* ipaddrstring, int portNum, char* buff, int bytes2send)
{
#ifdef _WIN32
    return 0;
#else
    memset(&sSendToAddr, 0, sizeof(struct sockaddr_in));
    sSendToAddr.sin_family = AF_INET;
    sSendToAddr.sin_port = htons(portNum);
    sSendToAddr.sin_addr.s_addr = inet_addr(ipaddrstring);

    // send the datagram
    int nBytesSent = sendto(nbSocket, buff, bytes2send, 0, (struct sockaddr*)&sSendToAddr, nReceiveAddrSize);
    //printf("Txd: %d Bytes\n", nBytesSent);
    if (nBytesSent >= 0)
        return nBytesSent;//printf("Rxd: %d Bytes\n", nBytesRecv);
    else if (errno == EWOULDBLOCK)
        return 0;//printf("Rxd: 0 Bytes\n");
    else
        throw "Txd: Error! %d\n", nBytesSent;
#endif
}
