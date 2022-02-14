#ifndef TCP_CLASS_H
#define TCP_CLASS_H

#include "version_config.hpp"

#ifdef _WIN32
#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")
#else
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (0)
#define WSAGetLastError() errno
#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEALREADY EALREADY
#define WSAEISCONN EISCONN
#define closesocket close
#define ioctlsocket ioctl
#define NO_ERROR (0)
#define ZeroMemory bzero
#endif
#include <stdint.h>
#include <stdio.h>

#ifdef DEBUG
#include <iostream>
#endif


enum tcpServerClientStatus
{
    tcpstat_uninitialized,
    tcpstat_created,
    tcpstat_error,
    tcpstat_connecting,
    tcpstat_connected
};


class tcp_class : public IODeviceClass
{
protected:
    enum tcpServerClientStatus tcpStatus = tcpstat_uninitialized;
    struct sockaddr_in serverAddr;
    int iResult;
    int iSendResult;
    int lastErr = 0;
    int historyLatch[10][2] = {{0} ,{0}};
    int latchDepth = 0;
#ifdef _WIN32
    WSADATA wsaData;
    SOCKET ClientSocket = INVALID_SOCKET;    
#else
    int ClientSocket = 0;
#endif
public:
    tcp_class();
    int Receive(char* buff, int len);
    int Send(char* buff, int len);  
    enum tcpServerClientStatus getTCPStatus();
    static const char* getTCPStatusString(enum tcpServerClientStatus gotTCPStatus);
};

class tcp_server_class : public tcp_class
{
protected:
#ifdef _WIN32
    SOCKET ListenSocket = INVALID_SOCKET;
#else
    int ListenSocket = 0;
#endif
public:
    tcp_server_class();
    void CreateNBServerSocket(uint16_t listenPortNum);
    void ServerListenAccept();
    void ShutdownRestartServerSocket(uint16_t listenPortNum);
};

class tcp_client_class : public tcp_class
{
protected:
#ifdef _WIN32
    
#else

#endif
public:
    tcp_client_class();
    void CreateNBClientSocket();
    void ShutdownRestartClientSocket(char* ipaddrstring, uint16_t serverPortNum);
};
#endif // TCP_CLASS_H
