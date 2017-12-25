#ifndef __CLIENTINFO_H_INCLUDED__
#define __CLIENTINFO_H_INCLUDED__
#include <string.h>
#include <iostream>
#include <sys/socket.h>

#include "ServerInfo.h"
class ClientInfo{

    //private:
    public:
        //friendList
        //groupList
        unsigned int m_myFd;
        clientState m_myState;
        std::string m_myName;

    public:
        ClientInfo(unsigned int clientFd);
        ClientInfo();
        ~ClientInfo();
        ClientInfo(const ClientInfo&);

        void setFd(unsigned int myFd){m_myFd = myFd;};
        void setState(clientState state){m_myState = state;};
        void setName(std::string name){m_myName = name;};
        void setName(const char* name){m_myName = name;};

        unsigned int getFd() const{return m_myFd;};
        clientState getState(){return m_myState;}
        std::string getName(){return m_myName;};
        char m_data[100];
        std::string m_dataToWrite;
};
#endif
