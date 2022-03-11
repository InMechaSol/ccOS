#ifndef TCP_CLASS_H
#define TCP_CLASS_H

#include "io_device.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdint>
#include <cstdio>
#else
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#endif


#ifdef _WIN32
#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
//#include <string>
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
//#include <cstring>
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

enum tcpServerClientStatus
{
    tcpstat_uninitialized = 0,
    tcpstat_created,
    tcpstat_error,
    tcpstat_connecting,
    tcpstat_connected
};
static const char* getTCPStatusString(enum tcpServerClientStatus gotTCPStatus);


struct tcpStruct
{
    enum tcpServerClientStatus tcpStatus;
    struct sockaddr_in serverAddr;
    int iResult;
    int iSendResult;
    int lastErr;
    int historyLatch[10][2];
    int latchDepth;
#ifdef _WIN32
    WSADATA wsaData;
    SOCKET ClientSocket;
#else
    int ClientSocket;
#endif
    struct devicedatastruct devdata; 
    char* ipaddrstring;
    uint16_t serverPortNum;
};
struct tcpStruct createTCPStruct();



struct tcpServerStruct
{
    struct tcpStruct tcpData;
    uint16_t listenPort;
    
#ifdef _WIN32
    SOCKET ListenSocket = INVALID_SOCKET;
#else
    int ListenSocket = 0;
#endif
};
struct tcpServerStruct createTCPServerStruct();

void TCPWrite(struct tcpStruct* tcpStructPtrIn);
void TCPRead(struct tcpStruct* tcpStructPtrIn);

void OpenTCPServer(struct tcpServerStruct* tcpServerStructPtrIn);
void shutdownRestartServerSocket(struct tcpServerStruct* tcpServerStructPtrIn);
void serverListenAccept(struct tcpServerStruct* tcpServerStructPtrIn);

void OpenTCPClient(struct tcpStruct* tcpStructPtrIn);
// written to be called from the mainloop() function
void shutdownRestartClientSocket(struct tcpStruct* tcpStructPtrIn);



#ifdef __cplusplus

class tcp_class : public IODeviceClass
{
protected:
    int closedevice();
    int readdevice();
    int writedevice();
    UI_8 isdeviceopen();
    struct tcpStruct* tcpDataPtr;

public:
    tcp_class(struct tcpStruct* tcpDataPtrIn);
    enum tcpServerClientStatus getTCPStatus();   
    struct tcpStruct* getTCPdataPtr();
};

class tcp_server_class : public tcp_class
{
protected:
    struct tcpServerStruct* tcpServerDataPtr;
    int opendevice();  
    

public:
    tcp_server_class(struct tcpServerStruct* tcpServerDataPtrIn);
    void ServerListenAccept();
    void ShutdownRestartServerSocket();
    
};

class tcp_client_class : public tcp_class
{
protected:
    int opendevice();   

public:
    tcp_client_class(struct tcpStruct* tcpDataPtrIn);
    void ShutdownRestartClientSocket();
};

#endif //!__cplusplus

#endif // !TCP_CLASS_H
