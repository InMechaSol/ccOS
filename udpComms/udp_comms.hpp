#ifndef UDP_CLASS_H
#define UDP_CLASS_H

#include "io_device.h"

#ifdef _WIN32
#include <winsock2.h>
#include <string>
#else
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#endif
#include <cstring>
#include <cstdint>
#include <cstdio>

struct udpStruct
{
    struct devicedatastruct devdata = createDeviceStruct();

    unsigned short bindPort;
    uint16_t partnerPortParsed;
    const char* ipaddrstring;
#ifdef _WIN32
    WSADATA wsaData;
    SOCKET nbSocket = INVALID_SOCKET;
    SOCKADDR_IN sReceiveFromAddr;
    SOCKADDR_IN sSendToAddr;
    int nReceiveAddrSize = sizeof(SOCKADDR_IN);
#else
    int nbSocket = 0;
    struct sockaddr_in sReceiveFromAddr;
    struct sockaddr_in sSendToAddr;
    socklen_t nReceiveAddrSize = sizeof(struct sockaddr_in);
#endif
};

class UDP_Class : public IODeviceClass
{
private:
    bool isServer = true;
    struct udpStruct* udpDataPtr = nullptr;
    void CreateNBSocketWindows();
    void CreateNBSocketLinux();
    int BindSocketWindows();
    int BindSocketLinux();
    int RecieveFromWindows();
    int RecieveFromLinux();
    int RecieveFromGetPortWindows();
    int RecieveFromGetPortLinux();
    int SendToWindows();
    int SendToLinux();
    bool socketCreated = false;
    bool socketBound = false;

protected:
    int opendevice();
    int closedevice();
    int readdevice();
    int writedevice();
    UI_8 isdeviceopen();

public:
    UDP_Class(struct udpStruct* udpDataPtrIn);    
    ~UDP_Class();
    void CreateNonBlockingSocket();
    int BindSocket();
    int SendTo();
    int ReceiveFrom();
    int ReceiveFromGetPort();
};







#endif // UDP_CLASS_H
