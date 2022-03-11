#include "tcp_comms.h"


struct tcpStruct createTCPStruct()
{
	struct tcpStruct outStruct;
    outStruct.devdata = createDeviceStruct();
#ifdef _WIN32
	outStruct.ClientSocket = INVALID_SOCKET;
#else
	outStruct.ClientSocket = 0;
#endif
    outStruct.latchDepth = 0;
	return outStruct;
}

struct tcpServerStruct createTCPServerStruct()
{
	struct tcpServerStruct outStruct;
	outStruct.tcpData = createTCPStruct();
#ifdef _WIN32
	outStruct.ListenSocket = INVALID_SOCKET;
#else
	outStruct.ListenSocket = 0;
#endif

	return outStruct;
}


#ifdef _WIN32
// All TCP
void TCPWrite(struct tcpStruct* tcpStructPtrIn)
{
#define writeAttempts (2)
    tcpStructPtrIn->devdata.numbytesWritten = 0;
    switch (tcpStructPtrIn->tcpStatus)
    {
    case tcpstat_connected:
        for (int i = 0; i < writeAttempts; i++)
        {
            if (tcpStructPtrIn->devdata.numbytes2Write > 0)
            {
                tcpStructPtrIn->iResult = send(tcpStructPtrIn->ClientSocket,
                    &tcpStructPtrIn->devdata.outbuff.charbuff[tcpStructPtrIn->devdata.numbytesWritten],
                    tcpStructPtrIn->devdata.numbytes2Write, 0);
                tcpStructPtrIn->lastErr = WSAGetLastError();
                if (tcpStructPtrIn->iResult < 0)
                {
                    if (tcpStructPtrIn->lastErr != WSAEWOULDBLOCK && tcpStructPtrIn->lastErr != EAGAIN)
                        tcpStructPtrIn->tcpStatus = tcpstat_error;
                    break;
                }
                else if (tcpStructPtrIn->iResult == 0)
                    break;
                else
                {
                    tcpStructPtrIn->devdata.numbytesWritten += tcpStructPtrIn->iResult;
                    tcpStructPtrIn->devdata.numbytes2Write -= tcpStructPtrIn->iResult;
                }
            }
            
        }
        return;
    default: return;
    }
}
void TCPRead(struct tcpStruct* tcpStructPtrIn)
{
#define readAttempts (2)
    tcpStructPtrIn->devdata.numbytesReadIn = 0;
    tcpStructPtrIn->devdata.numbytes2Read = charBuffMax;
    switch (tcpStructPtrIn->tcpStatus)
    {
    case tcpstat_connected:
        for (int i = 0; i < readAttempts; i++)
        {
            if (tcpStructPtrIn->devdata.numbytes2Read > 0)
            {
                tcpStructPtrIn->iResult = recv(tcpStructPtrIn->ClientSocket,
                    &tcpStructPtrIn->devdata.inbuff.charbuff[tcpStructPtrIn->devdata.numbytesReadIn],
                    tcpStructPtrIn->devdata.numbytes2Read, 0);
                tcpStructPtrIn->lastErr = WSAGetLastError();
                if (tcpStructPtrIn->iResult < 0) // nothing read this time
                {
                    if (tcpStructPtrIn->lastErr != WSAEWOULDBLOCK && tcpStructPtrIn->lastErr != EAGAIN)
                        tcpStructPtrIn->tcpStatus = tcpstat_error;
                    break;
                }
                else if (tcpStructPtrIn->iResult == 0) // nothing read this time
                    break;
                else // something read this time
                {
                    tcpStructPtrIn->devdata.numbytesReadIn += tcpStructPtrIn->iResult;
                    tcpStructPtrIn->devdata.numbytes2Read -= tcpStructPtrIn->iResult;
                }
            }
            
        }
        return;
    default: return;
    }
}

// Server TCP
void OpenTCPServer(struct tcpServerStruct* tcpServerStructPtrIn)
{
    // Initialize Winsock
    tcpServerStructPtrIn->tcpData.iResult = WSAStartup(MAKEWORD(2, 2), &tcpServerStructPtrIn->tcpData.wsaData);
    if (tcpServerStructPtrIn->tcpData.iResult != 0) {
        tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_error;
        return;
    }

    // Create a SOCKET for connecting to server
    tcpServerStructPtrIn->ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpServerStructPtrIn->ListenSocket == INVALID_SOCKET) {
        tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_error;
        return;
    }

    ZeroMemory(&tcpServerStructPtrIn->tcpData.serverAddr, sizeof(tcpServerStructPtrIn->tcpData.serverAddr));
    tcpServerStructPtrIn->tcpData.serverAddr.sin_family = AF_INET;
    tcpServerStructPtrIn->tcpData.serverAddr.sin_addr.s_addr = INADDR_ANY;
    tcpServerStructPtrIn->tcpData.serverAddr.sin_port = htons(tcpServerStructPtrIn->listenPort);

    // Setup the TCP listening socket
    tcpServerStructPtrIn->tcpData.iResult = bind(tcpServerStructPtrIn->ListenSocket,
        (struct sockaddr*)&tcpServerStructPtrIn->tcpData.serverAddr,
        sizeof(tcpServerStructPtrIn->tcpData.serverAddr));
    tcpServerStructPtrIn->tcpData.lastErr = WSAGetLastError();
    if (tcpServerStructPtrIn->tcpData.iResult == SOCKET_ERROR) {
        tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_error;
        return;
    }

    // set listen socket non-blocking
    unsigned long ul = 1;
    if (ioctlsocket(tcpServerStructPtrIn->ListenSocket, FIONBIO, (unsigned long*)&ul) != NO_ERROR)
    {
        tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_error;
        return;
    }

    tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_created;
}
// written to be called from the mainloop() function
void shutdownRestartServerSocket(struct tcpServerStruct* tcpServerStructPtrIn)
{
    switch (tcpServerStructPtrIn->tcpData.tcpStatus)
    {
    case tcpstat_error:
        // consider some error handling


        // transition state if the sockets are closed and ready to restart
        if (tcpServerStructPtrIn->tcpData.ClientSocket == INVALID_SOCKET && tcpServerStructPtrIn->ListenSocket == INVALID_SOCKET)
            tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_uninitialized;
        else
        {
            // close sockets etc
            closesocket(tcpServerStructPtrIn->ListenSocket);
            closesocket(tcpServerStructPtrIn->tcpData.ClientSocket);
            tcpServerStructPtrIn->tcpData.ClientSocket = INVALID_SOCKET;
            tcpServerStructPtrIn->ListenSocket = INVALID_SOCKET;
            WSACleanup();
        }
        if (tcpServerStructPtrIn->tcpData.tcpStatus == tcpstat_error)
            break;


    case tcpstat_uninitialized:
        return OpenTCPServer(tcpServerStructPtrIn);
    default: break;
    }
}
void serverListenAccept(struct tcpServerStruct* tcpServerStructPtrIn)
{
    switch (tcpServerStructPtrIn->tcpData.tcpStatus)
    {
    case tcpstat_created: // socket created, need to listen for client connection
        tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_connecting;
        tcpServerStructPtrIn->tcpData.lastErr = 0;
        tcpServerStructPtrIn->tcpData.iResult = SOCKET_ERROR;
    case tcpstat_connecting: // listen/accept being called cyclically until client connection
#ifdef DEBUGTCP
        std::cout << "\ntcp server: entering loop as: " << getTCPStatusString(tcpStatus) << " LastErr: " << lastErr << " iResult: " << iResult;
        std::cout.flush();
#endif
        if (tcpServerStructPtrIn->tcpData.lastErr == 0 && tcpServerStructPtrIn->tcpData.iResult == SOCKET_ERROR)
        {
            tcpServerStructPtrIn->tcpData.iResult = listen(tcpServerStructPtrIn->ListenSocket, 1);
            tcpServerStructPtrIn->tcpData.lastErr = WSAGetLastError();
#ifdef DEBUGTCP
            std::cout << "\ntcp server: listen call: lastErr = " << lastErr << " lSocket: " << ListenSocket << " iResult: " << iResult;
            std::cout.flush();
#endif
            if (tcpServerStructPtrIn->tcpData.latchDepth < 10)
            {
                tcpServerStructPtrIn->tcpData.historyLatch[tcpServerStructPtrIn->tcpData.latchDepth][0] = tcpServerStructPtrIn->tcpData.iResult;
                tcpServerStructPtrIn->tcpData.historyLatch[tcpServerStructPtrIn->tcpData.latchDepth][1] = tcpServerStructPtrIn->tcpData.lastErr;
                tcpServerStructPtrIn->tcpData.latchDepth++;
            }
            else
                tcpServerStructPtrIn->tcpData.latchDepth = tcpServerStructPtrIn->tcpData.latchDepth;
        }

        if (tcpServerStructPtrIn->tcpData.iResult == SOCKET_ERROR)
        {
#ifdef _WIN32
            if (tcpServerStructPtrIn->tcpData.lastErr != WSAEWOULDBLOCK)
#else
            if (lastErr != WSAEWOULDBLOCK && lastErr != EAGAIN)
#endif

            {
#ifdef DEBUGTCP
                std::cout << "\ntcp server: listen Error: ";
#endif
                tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_error;
                return;
            }

        }
        else
        {
            // Accept a client socket
            if (tcpServerStructPtrIn->tcpData.ClientSocket == SOCKET_ERROR || tcpServerStructPtrIn->tcpData.ClientSocket == INVALID_SOCKET)
            {
                tcpServerStructPtrIn->tcpData.ClientSocket = accept(tcpServerStructPtrIn->ListenSocket, nullptr, nullptr);
                tcpServerStructPtrIn->tcpData.lastErr = WSAGetLastError();

#ifdef DEBUGTCP
                std::cout << "\ntcp server: accept call: lastErr = " << lastErr << " cSocket: " << ClientSocket << " iResult: " << iResult;
                std::cout.flush();
#endif

                if (tcpServerStructPtrIn->tcpData.latchDepth < 10)
                {
                    tcpServerStructPtrIn->tcpData.historyLatch[tcpServerStructPtrIn->tcpData.latchDepth][0] = tcpServerStructPtrIn->tcpData.iResult;
                    tcpServerStructPtrIn->tcpData.historyLatch[tcpServerStructPtrIn->tcpData.latchDepth][1] = tcpServerStructPtrIn->tcpData.lastErr;
                    tcpServerStructPtrIn->tcpData.latchDepth++;
                }
                else
                    tcpServerStructPtrIn->tcpData.latchDepth = tcpServerStructPtrIn->tcpData.latchDepth;

                if ((tcpServerStructPtrIn->tcpData.ClientSocket == SOCKET_ERROR || tcpServerStructPtrIn->tcpData.ClientSocket == INVALID_SOCKET) && (tcpServerStructPtrIn->tcpData.lastErr != WSAEWOULDBLOCK && tcpServerStructPtrIn->tcpData.lastErr != EAGAIN))
                {
#ifdef DEBUGTCP
                    std::cout << "\ntcp server: accept Error: " << lastErr;
                    std::cout.flush();
#endif
                    tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_error;
                    return;
                }
            }
            else
            {
                // No longer need server socket
                closesocket(tcpServerStructPtrIn->ListenSocket);

                // set client socket non-blocking
                unsigned long ul = 1;
                if (ioctlsocket(tcpServerStructPtrIn->tcpData.ClientSocket, FIONBIO, (unsigned long*)&ul) != NO_ERROR)
                {
#ifdef DEBUGTCP
                    std::cout << "\ntcp server: that fucking ioctl ClientSocket: " << ClientSocket;
                    std::cout.flush();
#endif
                    tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_error;
                    return;
                }
                tcpServerStructPtrIn->tcpData.tcpStatus = tcpstat_connected;
            }

#ifdef DEBUGTCP
            std::cout << "\ntcp server: leaving loop as: " << getTCPStatusString(tcpStatus);
            std::cout.flush();
#endif
        }
        break;
    default: break;
    }
}

// Client TCP
void OpenTCPClient(struct tcpStruct* tcpStructPtrIn)
{
#ifdef DEBUGTCP
    std::cout << "starting again";
#endif
#ifdef _WIN32
    // Initialize Winsock
    tcpStructPtrIn->iResult = WSAStartup(MAKEWORD(2, 2), &tcpStructPtrIn->wsaData);
    if (tcpStructPtrIn->iResult != 0) {
        tcpStructPtrIn->tcpStatus = tcpstat_error;
        return;
    }
#endif
    // Create a SOCKET for connecting to server
    tcpStructPtrIn->ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpStructPtrIn->ClientSocket == INVALID_SOCKET) {
        tcpStructPtrIn->tcpStatus = tcpstat_error;
        return;
    }
    // set listen socket non-blocking
    unsigned long ul = 1;
    if (ioctlsocket(tcpStructPtrIn->ClientSocket, FIONBIO, (unsigned long*)&ul) != NO_ERROR)
    {
        tcpStructPtrIn->tcpStatus = tcpstat_error;
        return;
    }
    tcpStructPtrIn->tcpStatus = tcpstat_created;
}
void CloseTCPClient(struct tcpStruct* tcpStructPtrIn)
{
    ;
}
// written to be called from the mainloop() function
void shutdownRestartClientSocket(struct tcpStruct* tcpStructPtrIn)
{
    switch (tcpStructPtrIn->tcpStatus)
    {
    case tcpstat_error:
        // consider some error handling


        // transition state if the sockets are closed and ready to restart
        if (tcpStructPtrIn->ClientSocket == INVALID_SOCKET)
            tcpStructPtrIn->tcpStatus = tcpstat_uninitialized;
        else
        {
#ifdef DEBUGTCP
            std::cout << "\ntcp client: shutting down and...";
#endif
            // close socket etc
            closesocket(tcpStructPtrIn->ClientSocket);
#ifdef _WIN32
            WSACleanup();
#endif
            tcpStructPtrIn->ClientSocket = INVALID_SOCKET;
            break;
        }
    case tcpstat_uninitialized:
        return OpenTCPClient(tcpStructPtrIn);
    case tcpstat_created:
        tcpStructPtrIn->tcpStatus = tcpstat_connecting;
        tcpStructPtrIn->lastErr = 0;
    case tcpstat_connecting:

        // Connect to server.
#ifdef _WIN32	
        if (tcpStructPtrIn->lastErr == 0 || tcpStructPtrIn->lastErr == WSAEWOULDBLOCK || WSAEALREADY)
        {
#else
        if (lastErr == 0 || lastErr == WSAEWOULDBLOCK || lastErr == WSAEALREADY || lastErr == EAGAIN || lastErr == EINPROGRESS)
        {
#endif
            ZeroMemory(&tcpStructPtrIn->serverAddr, sizeof(tcpStructPtrIn->serverAddr));
            tcpStructPtrIn->serverAddr.sin_family = AF_INET;
            tcpStructPtrIn->serverAddr.sin_addr.s_addr = inet_addr(tcpStructPtrIn->ipaddrstring);
            tcpStructPtrIn->serverAddr.sin_port = htons(tcpStructPtrIn->serverPortNum);

            tcpStructPtrIn->iResult = connect(tcpStructPtrIn->ClientSocket, (struct sockaddr*)&tcpStructPtrIn->serverAddr, sizeof(tcpStructPtrIn->serverAddr));
            tcpStructPtrIn->lastErr = WSAGetLastError();

#ifdef DEBUGTCP
            std::cout << "\ntcp client: connecting()...lastErr: " << lastErr << " iResult: " << iResult;
#endif
            if (tcpStructPtrIn->latchDepth < 10)
            {
                tcpStructPtrIn->historyLatch[tcpStructPtrIn->latchDepth][0] = tcpStructPtrIn->iResult;
                tcpStructPtrIn->historyLatch[tcpStructPtrIn->latchDepth][1] = tcpStructPtrIn->lastErr;
                tcpStructPtrIn->latchDepth++;
            }
            else
            {
                tcpStructPtrIn->iResult = tcpStructPtrIn->iResult;
            }

            if (tcpStructPtrIn->iResult == SOCKET_ERROR)
            {
#ifdef _WIN32
                if (tcpStructPtrIn->lastErr != WSAEWOULDBLOCK && tcpStructPtrIn->lastErr != WSAEISCONN && tcpStructPtrIn->lastErr != WSAEALREADY)
#else	
                if (lastErr != WSAEWOULDBLOCK && lastErr != WSAEISCONN && lastErr != WSAEALREADY && lastErr != EAGAIN && lastErr != EINPROGRESS)
#endif
                {
                    tcpStructPtrIn->tcpStatus = tcpstat_error;

                }
                else if (tcpStructPtrIn->lastErr == WSAEISCONN)
                {
                    tcpStructPtrIn->tcpStatus = tcpstat_connected;
                    if (tcpStructPtrIn->ClientSocket == INVALID_SOCKET)
                        tcpStructPtrIn->tcpStatus = tcpstat_error;
                }

            }
            else
            {
                tcpStructPtrIn->tcpStatus = tcpstat_connected;
                if (tcpStructPtrIn->ClientSocket == INVALID_SOCKET)
                    tcpStructPtrIn->tcpStatus = tcpstat_error;
            }
        }
        break;
    default: break;
        }
    }


#else
void OpenTCPServer(struct tcpServerStruct* tcpServerStructPtrIn) { ; }
void CloseTCPServer(struct tcpServerStruct* tcpServerStructPtrIn) { ; }
void TCPServer_Write2Client(struct tcpServerStruct* tcpServerStructPtrIn) { ; }
void TCPServer_ReadFromClient(struct tcpServerStruct* tcpServerStructPtrIn) { ; }
void OpenTCPClient(struct tcpServerStruct* tcpServerStructPtrIn) { ; }
void CloseTCPClient(struct tcpServerStruct* tcpServerStructPtrIn) { ; }
void TCPClient_Write2Server(struct tcpServerStruct* tcpServerStructPtrIn) { ; }
void TCPClient_ReadFromServer(struct tcpServerStruct* tcpServerStructPtrIn) { ; }
#endif
