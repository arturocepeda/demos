
/*
    Arturo Cepeda Pérez

    Multiplayer (Win32)

    --- GEMultiplayer.cpp ---
*/

#include "GEMultiplayer.h"
#include <ws2tcpip.h>
#include <stdlib.h>

//
//  GEMultiplayer
//
int GEMultiplayer::getStatus()
{
    return iStatus;
}


//
//  GEClient
//
GEClient::GEClient()
{
    iStatus = 0;
    hSocket = INVALID_SOCKET;
    memset(&sServerAddress, 0, sizeof(sServerAddress));
}

GEClient::~GEClient()
{
}

void GEClient::init()
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

void GEClient::release()
{
    closesocket(hSocket);
    WSACleanup();
}

void GEClient::connectToServer(const char* IP, unsigned int Port)
{
    sServerAddress.sin_family = AF_INET;
    sServerAddress.sin_addr.s_addr = inet_addr(IP);
    sServerAddress.sin_port = htons(Port);

    if(connect(hSocket, (sockaddr*)&sServerAddress, sizeof(sServerAddress)) != 0)
        iStatus = -1;
}

void GEClient::sendMessage(const char* Message, unsigned int Size)
{
    sendto(hSocket, Message, Size, 0, (sockaddr*)&sServerAddress, sizeof(sServerAddress));
}

int GEClient::receiveMessage(char* Buffer, unsigned int MaxSize)
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
//  GEServer
//
GEServer::GEServer()
{
    iStatus = 0;
    hSocket = INVALID_SOCKET;
}

GEServer::~GEServer()
{
}

void GEServer::init()
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

void GEServer::release()
{
    closesocket(hSocket);
    WSACleanup();
}

void GEServer::activeServer(unsigned int Port)
{
    sockaddr_in sServerAddress;
    memset(&sServerAddress, 0, sizeof(sServerAddress));

    sServerAddress.sin_family = AF_INET;
    sServerAddress.sin_addr.s_addr = INADDR_ANY;
    sServerAddress.sin_port = htons(Port);

    if(bind(hSocket, (sockaddr*)&sServerAddress, sizeof(sServerAddress)) != 0)
        iStatus = -1;
}

void GEServer::sendMessage(const SRemoteAddress& ClientAddress, const char* Message, unsigned int Size)
{
    sockaddr_in sClientAddress;
    memset(&sClientAddress, 0, sizeof(sClientAddress));

    sClientAddress.sin_family = AF_INET;
    sClientAddress.sin_addr.s_addr = ClientAddress.IP;
    sClientAddress.sin_port = htons(ClientAddress.Port);

    sendto(hSocket, Message, Size, 0, (sockaddr*)&sClientAddress, sizeof(sClientAddress));
}

int GEServer::receiveMessage(SRemoteAddress* ClientAddress, char* Buffer, unsigned int MaxSize)
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

char* GEServer::getClientIP(unsigned int Client)
{
    in_addr sAddress;
    sAddress.s_addr = Client;

    return inet_ntoa(sAddress);
}
