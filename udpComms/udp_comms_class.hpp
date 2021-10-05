#ifndef UDP_CLASS_H
#define UDP_CLASS_H

#ifdef _WIN32
#include <winsock2.h>
#include <string>
#else
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#endif
#include <stdint.h>
#include <stdio.h>




class UDP_Class
{


private:
    void CreateNBSocketWindows();
    void CreateNBSocketLinux();
    int BindSocketWindows(unsigned short bindPort);
    int BindSocketLinux(unsigned short bindPort);
    int RecieveFromWindows(char* buff, int len);
    int RecieveFromLinux(char* buff, int len);
    int RecieveFromWindows(char* buff, int len, uint16_t* clientPortParsed);
    int RecieveFromLinux(char* buff, int len, uint16_t* clientPortParsed);
    int SendToWindows(char* ipaddrstring, int portNum, char* buff, int bytes2send);
    int SendToLinux(char* ipaddrstring, int portNum, char* buff, int bytes2send);
    bool socketCreated = false;
    bool socketBound = false;

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

public:
    UDP_Class();
    void CreateNonBlockingSocket();
    int BindSocket(unsigned short bindPort);
    int SendTo(char* ipaddrstring, int portNum, char* buff, int bytes2send);
    int ReceiveFrom(char* buff, int len); 
    int ReceiveFrom(char* buff, int len, uint16_t* clientPortParsed);
    ~UDP_Class();
};


// 7 lines and 28 characters per line
#define LargeLCDLineCharCount (28)
#define LargeLCDLineCount (7)
#define LargeLCDSize (LargeLCDLineCharCount*LargeLCDLineCount)

// 2 lines and 28 characters per line
#define SmallLCDLineCharCount (28)
#define SmallLCDLineCount (2)
#define SmallLCDSize (SmallLCDLineCharCount*SmallLCDLineCount)

// 10 lines and 28 characters per line
#define LineCharCount28 (28)
#define LineCount10 (10)
#define UISize2810 (LineCharCount28*LineCount10)

// 20 lines and 40 characters per line
#define LineCharCount40 (40)
#define LineCount20 (20)
#define UISize4020 (LineCharCount40*LineCount20)

// Selected Size used in Compilation
#define CompiledLineCharCount LineCharCount40
#define CompiledLineCount LineCount20
#define CompiledUISize UISize4020

enum UDPUIClientConnectionState
{
    UDPCommsInit = 0,
    UDPCOmmsDisconnected = 1,
    UDPCommsConnecting = 2,
    UDPCommsConnected = 3
};
enum userlevel
{
    usernone = 0,
    terminaloperator = 1,
    terminaladmin = 2,
    developer = 3,
    MAXUSERLEVEL = 4
};
enum uitype
{
    terminal = 0,
    largeLCD = 1,
    smallLCD = 2,
    MAXUITYPE = 3
};
class UDPUIServerClientClass : public UDP_Class
{
public:
    static const int UI_IP_buffSize = UISize4020;
    static const char UI_IP_AddrString[15];
    static const unsigned short UI_IP_ClientUI_BindPortArray[2];
    static const unsigned short UI_IP_ACU_BindPort = 50500;
    static const int UI_IP_MaxClientConnections = 2;
    static const int UI_IP_MSTimeout = 4000;
    static const char* getUDPUIClientConnectionStateString(enum UDPUIClientConnectionState theCS);
    static const char* getUDPUIClientUserLevelString(enum userlevel theUL);
    static const char* getUDPUIClientUITypeString(enum uitype theUIT);
protected:
    char inbuff[UI_IP_buffSize] = "";
    char outbuff[UI_IP_buffSize] = "";
    int bytesRecieved = 0;
    int bytes2Send = 0;
    int bytesSent = 0;
    unsigned short UI_IP_ClientUI_BindPort = UI_IP_ClientUI_BindPortArray[0];
    int mainLoopPerTimeOut = 0;
    
};






#endif // UDP_CLASS_H
