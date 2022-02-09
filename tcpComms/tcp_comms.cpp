#include "tcp_comms.hpp"
#include "os_execution_system.hpp"

#ifndef DEBUG
#ifdef DEBUGTCP
#undef DEBUGTCP
#endif
#else
//#define DEBUGTCP
#endif

tcp_class::tcp_class()
{

}
tcp_client_class::tcp_client_class()
{


}
tcp_server_class::tcp_server_class()
{

}

enum tcpServerClientStatus tcp_class::getTCPStatus()
{
    return tcpStatus;
}
const char* tcp_class::getTCPStatusString(enum tcpServerClientStatus gotTCPStatus)
{
    switch (gotTCPStatus)
    {
    case tcpstat_connected: return "connected";
    case tcpstat_connecting: return "connecting";
    case tcpstat_created: return "created";
    case tcpstat_error: return "error";
    case tcpstat_uninitialized: return "uninitialized";
    }
}

// written to be called from the setup() function
void tcp_server_class::CreateNBServerSocket(uint16_t listenPortNum)
{
#ifdef DEBUGTCP
    std::cout << "starting again";
#endif
#ifdef _WIN32
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        tcpStatus = tcpstat_error;
        return;
        //throw "WSAStartup failed with error: %d\n";
    }
#endif
    // Create a SOCKET for connecting to server
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        tcpStatus = tcpstat_error;
#ifdef DEBUGTCP
std::cout << "\ntcp server: listen socket: invalid after creation = " << lastErr;
#endif
        return;
    }

    ZeroMemory(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(listenPortNum);

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    lastErr = WSAGetLastError();
    if (iResult == SOCKET_ERROR) {
        tcpStatus = tcpstat_error;
#ifdef DEBUGTCP
std::cout << "\ntcp server: listen socket bind: lastErr = " << lastErr;
#endif
        return;
    }

    // set listen socket non-blocking
    unsigned long ul = 1;
    if (ioctlsocket(ListenSocket, FIONBIO, (unsigned long*)&ul) != NO_ERROR)
    {
        tcpStatus = tcpstat_error;
        return;
    }

    tcpStatus = tcpstat_created;
}

// written to be called from the setup() function
void tcp_client_class::CreateNBClientSocket()
{
#ifdef DEBUGTCP
    std::cout << "starting again";
#endif
#ifdef _WIN32
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        tcpStatus = tcpstat_error;
        return;
    }
#endif
    // Create a SOCKET for connecting to server
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        tcpStatus = tcpstat_error;
        return;
    }
    // set listen socket non-blocking
    unsigned long ul = 1;
    if (ioctlsocket(ClientSocket, FIONBIO, (unsigned long*)&ul) != NO_ERROR)
    {
        tcpStatus = tcpstat_error;
        return;
    }
    tcpStatus = tcpstat_created;
}

// written to be called from the mainloop() function
void tcp_server_class::ShutdownRestartServerSocket(uint16_t listenPortNum)
{
    switch (tcpStatus)
    {
    case tcpstat_error:
        // consider some error handling


        // transition state if the sockets are closed and ready to restart
        if(ClientSocket == INVALID_SOCKET && ListenSocket == INVALID_SOCKET)
            tcpStatus = tcpstat_uninitialized;
        else
        {
#ifdef DEBUGTCP
std::cout << "\ntcp server: shutting down and...";
#endif
            // close sockets etc
            closesocket(ListenSocket);
            closesocket(ClientSocket);
            ClientSocket = INVALID_SOCKET;
            ListenSocket = INVALID_SOCKET;
#ifdef _WIN32
            WSACleanup();
#endif
        }

    case tcpstat_uninitialized:
        return CreateNBServerSocket(listenPortNum);
    default: break;
    }
}

// written to be called from the mainloop() function
void tcp_client_class::ShutdownRestartClientSocket(char* ipaddrstring, uint16_t serverPortNum)
{
    switch (tcpStatus)
    {
    case tcpstat_error:
        // consider some error handling


        // transition state if the sockets are closed and ready to restart
        if (ClientSocket == INVALID_SOCKET)
            tcpStatus = tcpstat_uninitialized;
        else
        {
#ifdef DEBUGTCP
std::cout << "\ntcp client: shutting down and...";
#endif
            // close socket etc
            closesocket(ClientSocket);
#ifdef _WIN32
            WSACleanup();
#endif
            ClientSocket = INVALID_SOCKET;
            break;
        }
    case tcpstat_uninitialized:
        return CreateNBClientSocket();
    case tcpstat_created:
        tcpStatus = tcpstat_connecting;
        lastErr = 0;
    case tcpstat_connecting:

        // Connect to server.
#ifdef _WIN32	
		if (lastErr == 0 || lastErr == WSAEWOULDBLOCK || WSAEALREADY)
        {
#else
        if (lastErr == 0 || lastErr == WSAEWOULDBLOCK || lastErr == WSAEALREADY || lastErr == EAGAIN || lastErr == EINPROGRESS)
        {
#endif
            ZeroMemory(&serverAddr, sizeof(serverAddr));
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = inet_addr(ipaddrstring);
            serverAddr.sin_port = htons(serverPortNum);

            iResult = connect(ClientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            lastErr = WSAGetLastError();

#ifdef DEBUGTCP
            std::cout << "\ntcp client: connecting()...lastErr: "<<lastErr<<" iResult: "<<iResult;
#endif
            if (latchDepth < 10)
            {
                historyLatch[latchDepth][0] = iResult;
                historyLatch[latchDepth][1] = lastErr;
                latchDepth++;
            }
            else
            {
                iResult = iResult;
            }

            if (iResult == SOCKET_ERROR)
            {
#ifdef _WIN32
                if (lastErr != WSAEWOULDBLOCK && lastErr != WSAEISCONN && lastErr != WSAEALREADY)
#else	
				if (lastErr != WSAEWOULDBLOCK && lastErr != WSAEISCONN && lastErr != WSAEALREADY && lastErr != EAGAIN && lastErr != EINPROGRESS)
#endif
                {
                    tcpStatus = tcpstat_error;

                }
                else if (lastErr == WSAEISCONN)
                {
                    tcpStatus = tcpstat_connected;
                    if (ClientSocket == INVALID_SOCKET)
                        tcpStatus = tcpstat_error;
                }

            }
            else
            {
                tcpStatus = tcpstat_connected;
                if (ClientSocket == INVALID_SOCKET)
                    tcpStatus = tcpstat_error;
            }
        }
        break;
    default: break;
    }
}

// written to be called from the mainloop() function
void tcp_server_class::ServerListenAccept()
{
    switch (tcpStatus)
    {
    case tcpstat_created: // socket created, need to listen for client connection
        tcpStatus = tcpstat_connecting;
        lastErr = 0;
        iResult = SOCKET_ERROR;
    case tcpstat_connecting: // listen/accept being called cyclically until client connection
#ifdef DEBUGTCP
std::cout << "\ntcp server: entering loop as: "<<getTCPStatusString(tcpStatus)<<" LastErr: "<<lastErr<<" iResult: "<<iResult;
std::cout.flush();
#endif
        if (lastErr == 0 && iResult == SOCKET_ERROR)
        {
            iResult = listen(ListenSocket, 1);
            lastErr = WSAGetLastError();
#ifdef DEBUGTCP
std::cout << "\ntcp server: listen call: lastErr = " << lastErr << " lSocket: " << ListenSocket << " iResult: " << iResult;
std::cout.flush();
#endif
            if (latchDepth < 10)
            {
                historyLatch[latchDepth][0] = iResult;
                historyLatch[latchDepth][1] = lastErr;
                latchDepth++;
            }
            else
                latchDepth = latchDepth;
        }

        if (iResult == SOCKET_ERROR)
        {
#ifdef _WIN32
			if (lastErr != WSAEWOULDBLOCK)
#else
			if (lastErr != WSAEWOULDBLOCK && lastErr != EAGAIN)
#endif
            
            {
#ifdef DEBUGTCP
std::cout << "\ntcp server: listen Error: ";
#endif
                tcpStatus = tcpstat_error;
                return;
            }

        }
        else
        {
            // Accept a client socket
            if (ClientSocket == SOCKET_ERROR || ClientSocket == INVALID_SOCKET)
            {
                ClientSocket = accept(ListenSocket, nullptr, nullptr);
                lastErr = WSAGetLastError();

                #ifdef DEBUGTCP
                std::cout << "\ntcp server: accept call: lastErr = " << lastErr << " cSocket: " << ClientSocket << " iResult: " << iResult;
                std::cout.flush();
                #endif

                if (latchDepth < 10)
                {
                    historyLatch[latchDepth][0] = iResult;
                    historyLatch[latchDepth][1] = lastErr;
                    latchDepth++;
                }
                else
                    latchDepth = latchDepth;

                if((ClientSocket == SOCKET_ERROR || ClientSocket == INVALID_SOCKET) && (lastErr != WSAEWOULDBLOCK && lastErr != EAGAIN))
                {
#ifdef DEBUGTCP
std::cout << "\ntcp server: accept Error: "<<lastErr;
std::cout.flush();
#endif
                    tcpStatus = tcpstat_error;
                    return;
                }
            }
            else
            {
                // No longer need server socket
                closesocket(ListenSocket);

                // set client socket non-blocking
                unsigned long ul = 1;
                if (ioctlsocket(ClientSocket, FIONBIO, (unsigned long*)&ul) != NO_ERROR)
                {
#ifdef DEBUGTCP
std::cout << "\ntcp server: that fucking ioctl ClientSocket: "<<ClientSocket;
std::cout.flush();
#endif
                    tcpStatus = tcpstat_error;
                    return;
                }
                tcpStatus = tcpstat_connected;
            }

#ifdef DEBUGTCP
std::cout << "\ntcp server: leaving loop as: "<<getTCPStatusString(tcpStatus);
std::cout.flush();
#endif
        }
        break;
    default: break;
    }
}

// written to be called from the mainloop() function
int tcp_class::Receive(char* buff, int len)
{
    #define readAttempts (2)
    int outs[readAttempts] = {0};
    int bytesread = 0;
    int len2read = len;
    switch (tcpStatus)
    {
    case tcpstat_connected:
        for (int i = 0; i < readAttempts; i++)
        {
            outs[i] = recv(ClientSocket, &buff[bytesread], len2read, 0);
            lastErr = WSAGetLastError();
            if (outs[i] < 0)
            {
                if (lastErr != WSAEWOULDBLOCK && lastErr != EAGAIN)
                    tcpStatus = tcpstat_error;
                break;
            }
            else if (outs[i] == 0)
                break;
            else if (i > 0)
            {
                outs[0] += outs[i];
                len2read -= outs[i];
            }
        }        
        return outs[0];
    default: return 0;
    }
}

// written to be called from the mainloop() function
int tcp_class::Send(char* buff, int len)
{
#define writeAttempts (2)
    int outs[writeAttempts] = { 0 };
    int byteswrritten = 0;
    int len2write = len;
    switch (tcpStatus)
    {
    case tcpstat_connected:
        for (int i = 0; i < writeAttempts; i++)
        {
            outs[i] = send(ClientSocket, &buff[byteswrritten], len2write, 0);
            lastErr = WSAGetLastError();
            if (outs[i] < 0)
            {
                if (lastErr != WSAEWOULDBLOCK && lastErr != EAGAIN)
                    tcpStatus = tcpstat_error;
                break;
            }
            else if (outs[i] == 0)
                break;
            else if (i > 0)
            {
                outs[0] += outs[i];
                len2write -= outs[i];
            }
        }
        return outs[0];
    default: return 0;
    }
}
