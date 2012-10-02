
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
    SOCKET hSocket;
    int iStatus;

public:
    void init();
    void release();

    int getStatus();
};

class GEClient : public GEMultiplayer
{
private:
    sockaddr_in sServerAddress;

public:
    GEClient();
    ~GEClient();

    void connectToServer(const char* IP, unsigned int Port);

    void sendMessage(const char* Message, unsigned int Size);
    int receiveMessage(char* Buffer, unsigned int MaxSize);
};

class GEServer : public GEMultiplayer
{
public:
    GEServer();
    ~GEServer();

    void activeServer(unsigned int Port);

    void sendMessage(const SRemoteAddress& ClientAddress, const char* Message, unsigned int Size);
    int receiveMessage(SRemoteAddress* ClientAddress, char* Buffer, unsigned int MaxSize);

    char* getClientIP(unsigned int Client);
};

#endif
