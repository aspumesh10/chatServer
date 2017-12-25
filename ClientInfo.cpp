
#include <string.h>
#include"ClientInfo.h"
#define MIN(a,b) a>=b?a:b
ClientInfo::ClientInfo(unsigned int clientFd=0):m_myFd(clientFd)
{
    m_myState = clientState::inRegister;
    m_myName = "unknown";
    memset(m_data,0,sizeof m_data);
   // memset(m_dataToWrite,0, sizeof m_dataToWrite);
}

ClientInfo::~ClientInfo()
{
	m_myFd = 0x00;
}

//for vector
ClientInfo::ClientInfo(const ClientInfo& cli)
{
    this->m_myFd = cli.m_myFd;
   // memcpy(this->m_data,cli.m_data,MIN(sizeof this->m_data, sizeof cli.m_data ));
    memcpy ((void*)cli.m_data,this->m_data, sizeof cli.m_data);
    //memcpy ((void*)cli.m_dataToWrite,this->m_dataToWrite, sizeof cli.m_data);
    this->m_dataToWrite = cli.m_dataToWrite;
    this->m_myName = cli.m_myName;
    this->m_myState = cli.m_myState;
}
