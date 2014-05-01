
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Multiplayer
//
//  --- GEMultiplayer.h ---
//
//////////////////////////////////////////////////////////////////

#pragma once

struct SRemoteAddress
{
    unsigned int IP;
    unsigned int Port;
};

class GEClient
{
public:
    virtual ~GEClient();

    virtual void connectToServer(const char* IP, unsigned int Port) = 0;

    virtual void sendMessage(const char* Message, unsigned int Size) = 0;
    virtual int receiveMessage(char* Buffer, unsigned int MaxSize) = 0;
};

class GEServer
{
public:
    virtual ~GEServer();

    virtual void activeServer(unsigned int Port) = 0;

    virtual void sendMessage(const SRemoteAddress& ClientAddress, const char* Message, unsigned int Size) = 0;
    virtual int receiveMessage(SRemoteAddress* ClientAddress, char* Buffer, unsigned int MaxSize) = 0;

    virtual char* getClientIP(unsigned int Client) = 0;
};
