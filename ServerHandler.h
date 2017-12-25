#ifndef __SERVERHANDLER_H_INCLUDED__
#define __SERVERHANDLER_H_INCLUDED__

#include <string.h>
#include "Server.h"
#include "ServerInfo.h"
class ServerHandler
{
    private:
    static bool readSuccessful;
    Server *m_chatServer;

    public:
    /*static*/ //Server* m_chatServer;
    ServerHandler(Server* chatServer);
    ~ServerHandler();
    static void onData(int signal);
    void init();

};

#endif
