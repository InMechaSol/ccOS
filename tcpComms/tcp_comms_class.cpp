#include "tcp_comms.c"

#ifndef DEBUG
#ifdef DEBUGTCP
#undef DEBUGTCP
#endif
#else
//#define DEBUGTCP
#endif

tcp_class::tcp_class(struct tcpStruct* tcpDataPtrIn):IODeviceClass(&tcpDataPtrIn->devdata)
{
    tcpDataPtr = tcpDataPtrIn;
}
tcp_client_class::tcp_client_class(struct tcpStruct* tcpDataPtrIn):tcp_class(tcpDataPtrIn)
{


}
tcp_server_class::tcp_server_class(struct tcpServerStruct* tcpServerDataPtrIn) : tcp_class(&tcpServerDataPtrIn->tcpData)
{
    tcpServerDataPtr = tcpServerDataPtrIn;
}

enum tcpServerClientStatus tcp_class::getTCPStatus()
{
    return tcpDataPtr->tcpStatus;
}
int tcp_class::closedevice() { return 0; }
UI_8 tcp_class::isdeviceopen() { return ui8FALSE; }
int tcp_class::readdevice() 
{
    TCPRead(tcpDataPtr);
    return tcpDataPtr->devdata.numbytesReadIn;
}
int tcp_class::writedevice() 
{
    TCPWrite(tcpDataPtr);
    return tcpDataPtr->devdata.numbytesWritten;
}


int tcp_server_class::opendevice()
{
    OpenTCPServer(tcpServerDataPtr);
    return tcpServerDataPtr->tcpData.iResult;
}
// written to be called from the mainloop() function
void tcp_server_class::ShutdownRestartServerSocket()
{
    return shutdownRestartServerSocket(tcpServerDataPtr);
}
// written to be called from the mainloop() function
void tcp_server_class::ServerListenAccept()
{
    return serverListenAccept(tcpServerDataPtr);
}



int tcp_client_class::opendevice()
{
    OpenTCPClient(tcpDataPtr);
    return tcpDataPtr->iResult;
}
// written to be called from the mainloop() function
void tcp_client_class::ShutdownRestartClientSocket()
{
    return shutdownRestartClientSocket(tcpDataPtr);
}


