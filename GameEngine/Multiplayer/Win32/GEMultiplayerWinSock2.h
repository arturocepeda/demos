
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Multiplayer (WinSock2)
//
//  --- GEMultiplayerWinSock2.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

#include "Multiplayer/GEMultiplayer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>

class GEWinSock2
{
protected:
    SOCKET hSocket;
    int iStatus;

    void init();
    void release();
};

class GEClientWinSock2 : public GEClient, public GEWinSock2
{
private:
    sockaddr_in sServerAddress;

public:
    GEClientWinSock2();
    ~GEClientWinSock2();

    void connectToServer(const char* IP, unsigned int Port) override;

    void sendMessage(const char* Message, unsigned int Size) override;
    int receiveMessage(char* Buffer, unsigned int MaxSize) override;
};

class GEServerWinSock2 : public GEServer, public GEWinSock2
{
public:
    GEServerWinSock2();
    ~GEServerWinSock2();

    void activeServer(unsigned int Port) override;

    void sendMessage(const SRemoteAddress& ClientAddress, const char* Message, unsigned int Size) override;
    int receiveMessage(SRemoteAddress* ClientAddress, char* Buffer, unsigned int MaxSize) override;

    char* getClientIP(unsigned int Client) override;
};
