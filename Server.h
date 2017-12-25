#ifndef __SERVER_H_INCLUDED__
#define __SERVER_H_INCLUDED__
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <algorithm>
#include "ClientInfo.h"
#include "ServerInfo.h"

class Server{
	
	private:
    unsigned short m_sockFd;
	unsigned short m_newSockFd;
	unsigned int m_port;
    unsigned int m_highSock;
    //static unsigned int m_connectlist[];
    std::vector<unsigned int> m_connectList;
    static bool goToWrite;
    fd_set m_socksReadFds;
    fd_set m_socksWriteFds;

	public:
	explicit Server(unsigned int port=3000);
	bool createServer();
	bool run();
    void readSocketActivity();
    void buildSelectList();
    void checkNewConnection();
    void checkData(unsigned int listNumber);
    //std::vector<ClientInfo*> m_clientInfoList;
    ClientInfo* m_clientInfoList[10];
    static void writeUnsolicitedData(int signal);
    void writeData();
    ClientInfo *m_cliInfo;
    std::map<unsigned int, ClientInfo*> m_clientMap;
    void broadCast(int socket, const char* data, int length);
};
#endif
