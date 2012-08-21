
/*
    Arturo Cepeda Pérez

    Multiplayer (Win32)

    --- GEMultiplayer.h ---    
*/

// Library links: ws2_32.lib

#ifndef _GEMULTIPLAYER_H_
#define _GEMULTIPLAYER_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>

struct SRemoteAddress
{
    unsigned int IP;
    unsigned int Port;
};

class GEMultiplayer
{
protected:
    int iStatus;

public:
    virtual void init() = 0;
    virtual void release() = 0;

    int getStatus();
};

class GEClient : public GEMultiplayer
{
private:
    SOCKET hSocket;
    sockaddr_in sServerAddress;

public:
    GEClient();
    ~GEClient();

    void init();
    void release();

    void connectToServer(const char* IP, unsigned int Port);

    void sendMessage(const char* Message, unsigned int Size);
    int receiveMessage(char* Buffer, unsigned int MaxSize);
};

class GEServer : public GEMultiplayer
{
private:
    SOCKET hSocket;

public:
    GEServer();
    ~GEServer();

    void init();
    void release();

    void activeServer(unsigned int Port);

    void sendMessage(const SRemoteAddress& ClientAddress, const char* Message, unsigned int Size);
    int receiveMessage(SRemoteAddress* ClientAddress, char* Buffer, unsigned int MaxSize);

    char* getClientIP(unsigned int Client);
};

#endif
