#include "udp_comms.hpp"
#include "os_execution_system.hpp"


UDP_Class::UDP_Class(struct udpStruct* udpDataPtrIn):IODeviceClass(&udpDataPtrIn->devdata)
{
  udpDataPtr = udpDataPtrIn;
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

int UDP_Class::BindSocket()
{
    int retVal = -1;
    if (!socketBound)
    {
#ifdef _WIN32
        retVal = BindSocketWindows(bindPort);
#else
        retVal = BindSocketLinux();
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
        close(udpDataPtr->nbSocket);
#endif
}

int UDP_Class::ReceiveFrom()
{
    if (socketCreated && socketBound)
    {
#ifdef _WIN32
        return RecieveFromWindows();
#else
        return RecieveFromLinux();
#endif
    }
    else
        return 0;
	
}

int UDP_Class::ReceiveFromGetPort()
{
    if (socketCreated && socketBound)
    {
#ifdef _WIN32
        return RecieveFromWindows();
#else
        return RecieveFromLinux();
#endif
    }
    else
        return 0;

}

int UDP_Class::SendTo()
{
    if (socketCreated)
    {
#ifdef _WIN32
        return SendToWindows();
#else
        return SendToLinux();
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
    

    udpDataPtr->nbSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (udpDataPtr->nbSocket == 0)
        throw "Error at Socket()\n";

    int nRet = ioctl(udpDataPtr->nbSocket, FIONBIO, (unsigned long*)&ul);
    if (nRet != 0)
        throw "Error at ioctlsocket()";
#endif
}



int UDP_Class::BindSocketWindows()
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

int UDP_Class::BindSocketLinux()
{
#ifdef _WIN32
    return 0;
#else
    int boundPort = udpDataPtr->bindPort;

    while (boundPort != 0)
    {
        memset(&udpDataPtr->sReceiveFromAddr, 0, sizeof(struct sockaddr_in));
        udpDataPtr->sReceiveFromAddr.sin_family = AF_INET;
        udpDataPtr->sReceiveFromAddr.sin_port = htons(boundPort);
        udpDataPtr->sReceiveFromAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(udpDataPtr->nbSocket, (struct sockaddr*)&udpDataPtr->sReceiveFromAddr, sizeof(struct sockaddr_in)) < 0)
        {
            if (errno == EADDRINUSE)
            {
                if (boundPort - udpDataPtr->bindPort > 1)
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



int UDP_Class::RecieveFromWindows()
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

int UDP_Class::RecieveFromLinux()
{
#ifdef _WIN32
    return 0;
#else

    // Get the datagram
    int nBytesRecv = recvfrom(udpDataPtr->nbSocket, &udpDataPtr->devdata.inbuff.bytebuff[udpDataPtr->devdata.numbytesReadIn], udpDataPtr->devdata.numbytes2Read, 0,(struct sockaddr*)&udpDataPtr->sReceiveFromAddr, &udpDataPtr->nReceiveAddrSize);
    if (nBytesRecv >= 0)
        return nBytesRecv;//printf("Rxd: %d Bytes\n", nBytesRecv);
    else if (errno == EWOULDBLOCK)
        return 0;//printf("Rxd: 0 Bytes\n");
    else
        throw "Rxd: Error! %d\n", nBytesRecv;
#endif

}

int UDP_Class::RecieveFromGetPortWindows()
{
    int bytesRecieved = RecieveFromWindows( );
    udpDataPtr->partnerPortParsed = udpDataPtr->sReceiveFromAddr.sin_port;
    return bytesRecieved;
}

int UDP_Class::RecieveFromGetPortLinux()
{
    int bytesRecieved = RecieveFromLinux();
    udpDataPtr->partnerPortParsed = udpDataPtr->sReceiveFromAddr.sin_port;
    return bytesRecieved;
}


int UDP_Class::SendToWindows()
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

int UDP_Class::SendToLinux()
{
#ifdef _WIN32
    return 0;
#else
    memset(&udpDataPtr->sSendToAddr, 0, sizeof(struct sockaddr_in));
    udpDataPtr->sSendToAddr.sin_family = AF_INET;
    udpDataPtr->sSendToAddr.sin_port = htons(udpDataPtr->partnerPortParsed);
    udpDataPtr->sSendToAddr.sin_addr.s_addr = inet_addr(udpDataPtr->ipaddrstring);

    // send the datagram
    int nBytesSent = sendto(udpDataPtr->nbSocket, &udpDataPtr->devdata.outbuff.bytebuff[udpDataPtr->devdata.numbytesWritten], udpDataPtr->devdata.numbytes2Write, 0, (struct sockaddr*)&udpDataPtr->sSendToAddr, udpDataPtr->nReceiveAddrSize);
    //printf("Txd: %d Bytes\n", nBytesSent);
    if (nBytesSent >= 0)
        return nBytesSent;//printf("Rxd: %d Bytes\n", nBytesRecv);
    else if (errno == EWOULDBLOCK)
        return 0;//printf("Rxd: 0 Bytes\n");
    else
        throw "Txd: Error! %d\n", nBytesSent;
#endif
}

int UDP_Class::opendevice()
{
    CreateNonBlockingSocket();
    return 0;
}
int UDP_Class::closedevice()
{
    return 0;
}
int UDP_Class::readdevice()
{
    if(isServer)
        return ReceiveFrom();
    else
        return ReceiveFromGetPort();
}
int UDP_Class::writedevice()
{
    return SendTo();
}
UI_8 UDP_Class::isdeviceopen()
{
    return ui8FALSE;
}
