
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Multiplayer (WinSock2)
//
//  --- GEMultiplayerWinSock2.cpp ---
//
//////////////////////////////////////////////////////////////////

#include "GEMultiplayerWinSock2.h"
#include <ws2tcpip.h>
#include <stdlib.h>

//
//  GEWinSock2
//
void GEWinSock2::init()
{
    // initialize winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // create socket
    hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(hSocket == INVALID_SOCKET)
    {
        iStatus = -1;
        return;
    }

    // set non-blocking mode
    u_long iMode = 1;
    ioctlsocket(hSocket, FIONBIO, &iMode);
}

void GEWinSock2::release()
{
    closesocket(hSocket);
    WSACleanup();
}


//
//  GEClientWinSock2
//
GEClientWinSock2::GEClientWinSock2()
{
    memset(&sServerAddress, 0, sizeof(sServerAddress));
    init();
}

GEClientWinSock2::~GEClientWinSock2()
{
    release();
}

void GEClientWinSock2::connectToServer(const char* IP, unsigned int Port)
{
    sServerAddress.sin_family = AF_INET;
    sServerAddress.sin_addr.s_addr = inet_addr(IP);
    sServerAddress.sin_port = htons(Port);

    if(connect(hSocket, (sockaddr*)&sServerAddress, sizeof(sServerAddress)) != 0)
        iStatus = -1;
}

void GEClientWinSock2::sendMessage(const char* Message, unsigned int Size)
{
    sendto(hSocket, Message, Size, 0, (sockaddr*)&sServerAddress, sizeof(sServerAddress));
}

int GEClientWinSock2::receiveMessage(char* Buffer, unsigned int MaxSize)
{
    int iBytes;
    sockaddr_in sRemoteAddress;
    int iFromLen = sizeof(sRemoteAddress);

    iBytes = recvfrom(hSocket, Buffer, MaxSize, 0, (sockaddr*)&sRemoteAddress, &iFromLen);

    if(sRemoteAddress.sin_addr.s_addr == sServerAddress.sin_addr.s_addr)
        return iBytes;
    else
        return 0;
}


//
//  GEServerWinSock2
//
GEServerWinSock2::GEServerWinSock2()
{
    init();
}

GEServerWinSock2::~GEServerWinSock2()
{
    release();
}

void GEServerWinSock2::activeServer(unsigned int Port)
{
    sockaddr_in sServerAddress;
    memset(&sServerAddress, 0, sizeof(sServerAddress));

    sServerAddress.sin_family = AF_INET;
    sServerAddress.sin_addr.s_addr = INADDR_ANY;
    sServerAddress.sin_port = htons(Port);

    if(bind(hSocket, (sockaddr*)&sServerAddress, sizeof(sServerAddress)) != 0)
        iStatus = -1;
}

void GEServerWinSock2::sendMessage(const SRemoteAddress& ClientAddress, const char* Message, unsigned int Size)
{
    sockaddr_in sClientAddress;
    memset(&sClientAddress, 0, sizeof(sClientAddress));

    sClientAddress.sin_family = AF_INET;
    sClientAddress.sin_addr.s_addr = ClientAddress.IP;
    sClientAddress.sin_port = htons(ClientAddress.Port);

    sendto(hSocket, Message, Size, 0, (sockaddr*)&sClientAddress, sizeof(sClientAddress));
}

int GEServerWinSock2::receiveMessage(SRemoteAddress* ClientAddress, char* Buffer, unsigned int MaxSize)
{
    int iBytes;
    sockaddr_in sClientAddress;
    int iFromLen = sizeof(sClientAddress);

    iBytes = recvfrom(hSocket, Buffer, MaxSize, 0, (sockaddr*)&sClientAddress, &iFromLen);    

    if(iBytes > 0)
    {
        ClientAddress->IP = sClientAddress.sin_addr.s_addr;
        ClientAddress->Port = htons(sClientAddress.sin_port);
    }

    return iBytes;
}

char* GEServerWinSock2::getClientIP(unsigned int Client)
{
    in_addr sAddress;
    sAddress.s_addr = Client;

    return inet_ntoa(sAddress);
}
