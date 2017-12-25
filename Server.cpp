//#include <csignal>
#include <sys/signal.h>
#include "Server.h"

using namespace std;
const char MAX_LISTEN = 0x0A;

//unsigned int Server::m_connectList[100];
bool Server::goToWrite = false;

Server::Server(unsigned int port):m_port(port)
{
	m_sockFd = 0x00;
	m_newSockFd = 0x00;
    m_highSock = 0x00;
    m_cliInfo = NULL;
    memset(m_clientInfoList, '\0', sizeof m_clientInfoList);
    signal(SIGUSR2, Server::writeUnsolicitedData);
     //m_connectList(10);
 }

bool Server::createServer()
{

	bool ret = false;
    int reuse_addr = 1;
    /* Used so we can re-bind to our port
       while a previous connection is still
       in TIME_WAIT state. */
    struct sockaddr_in server_addr;
	m_sockFd =  socket(AF_INET, SOCK_STREAM, 0);
    /* So that we can re-bind to it without TIME_WAIT problems */
    setsockopt(m_sockFd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,sizeof(reuse_addr));
	if (m_sockFd < 0) 
	{ 	
		
		cout<<"Socket creation failed"<<endl;
	}
	else
	{
//memset
        bzero((char *) &server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(m_port);
		if (bind(m_sockFd, (struct sockaddr *) &server_addr,sizeof(server_addr)) < 0)
		{
			cout<<"bind failed"<<endl;	
		} 
		else if (listen(m_sockFd ,MAX_LISTEN ) <0)
		{
			cout<<"Listening failed"<<endl;
		}
		else 
		{	
            m_highSock = m_sockFd;

			ret = true;
			cout<<"create server success"<<endl;		
		}
	}
	return ret;
		
}

void Server::buildSelectList()
{
        FD_ZERO(&m_socksReadFds);
        FD_SET(m_sockFd,&m_socksReadFds);

        for(std::vector<unsigned int>::iterator itr= m_connectList.begin();
             itr != m_connectList.end(); itr++)
        {
            FD_SET(*itr,&m_socksReadFds);
            FD_SET(*itr,&m_socksWriteFds);
            if(*itr > m_highSock)
                m_highSock = *itr;
        }
    
}

void Server::checkNewConnection()
{
    int connection;
    clientState currentClientState;
    bool gotRoom = false;
    connection = accept(m_sockFd, NULL, NULL);
	if (connection < 0) {
        cout<<"Accept failed "<<endl;
		exit(0); ///exit the accept failed
	}

    //check return type
    int options = fcntl(connection,F_GETFL);
    options = (options | O_NONBLOCK);
    fcntl(connection,F_SETFL,options);
    //accept success
    m_connectList.push_back((unsigned int)connection);
    m_cliInfo = NULL;
    m_cliInfo = new ClientInfo(connection);
    if(m_cliInfo && true/*registerClient(connection)*/)
    {
        //inserts m_cliInfo inside with key connection
        m_clientMap[connection]=m_cliInfo;
        Server::goToWrite = true;//for registration
        writeData();
        gotRoom = true;
        cout<<"got room for new client"<<endl;
    }

    if (gotRoom == false)
    {
        m_connectList.pop_back();
        cout<<"No room left for new client"<<endl;
        close(connection);
    }
}

//hanfle == 0 condition
void Server::checkData(unsigned int checkSocket)
{
    char buffer[100];
    memset(buffer, '\0', sizeof buffer);
    int result = read(checkSocket,buffer,sizeof(buffer));
    if ( result <= 0)
    {
        //cleanup
        m_connectList.erase(std::remove(m_connectList.begin(), m_connectList.end(),checkSocket),m_connectList.end());
        m_clientMap.erase(checkSocket);
        close(checkSocket);
    }
    else
    {
        std::cout<<"#############the read result"<<result<<endl;
       // buffer[result-2]='\0';

        if(m_clientMap[checkSocket]->getState() == clientState::idle)
            m_clientMap[checkSocket]->setState(clientState::readable);
        else
            buffer[result-2]='\0';

        memset(m_clientMap[checkSocket]->m_data,0,sizeof m_clientMap[checkSocket]->m_data);
        memcpy(m_clientMap[checkSocket]->m_data,buffer,sizeof buffer);
        raise(SIGUSR1);
    }

    //check for exception as if element not present it throws one
}

void Server::writeUnsolicitedData(int signal)
{
    cout<<"inside the writedata"<<endl;
    Server::goToWrite = true;
}

void Server::writeData()
{
 // int result;
    if(Server::goToWrite == true)
    {
        Server::goToWrite =false;
        for(std::map<unsigned int , ClientInfo*>::iterator itr=m_clientMap.begin();
            itr!=m_clientMap.end();
            itr++)
        {

                if( itr->second->getState()==clientState::inRegister)
                {
                        char buff[]="please Enter your Name for registration:";
                        write(itr->first,buff,sizeof buff);
                        itr->second->setState(clientState::getName);
                        //raise(SIGUSR1);
                        break;
                }
                if( itr->second->getState() == clientState::writable)
                {
                    itr->second->setState(clientState::idle);
                    broadCast(itr->first,(itr->second->m_dataToWrite).c_str(),itr->second->m_dataToWrite.length());
                    break;
                }
        }
    }

}

void Server::broadCast(int socket,const char* data, int length)
{
    std::cout<<"broadCasst:::data-->"<<data<<"sizeof list"<<m_connectList.size()<<endl;
    if(data != NULL)
    {
        //for (all entries in queue)
        for(std::vector<unsigned int>::iterator itr= m_connectList.begin(); itr != m_connectList.end(); itr++)
        {
           //use select isset
           //do not send me back
           if(*itr != socket)
             write(*itr,data,length);
        }
    }
}

/*
void Server::removeFromVectorList(unsigned int fd)
{
    std::vector<ClientInfo*>::const_iterator itr;

    for(itr=m_clientInfoList.end(); itr!=m_clientInfoList.begin(); --itr){
        if((*itr)->getFd() == fd){
            //m_clientInfoList.pop_back();
            m_clientInfoList.erase(itr);
            //pop back and delete
            break;
        }
    }


}
*/
/*
void Server::sendDataToHandler(const char *buffer)
{
    union sigval value;
    unsigned int toSendPid = getpid(); //pid of handler currently single
                                       //process use current proc id
    char *data = new char(sizeof buffer + 1);
    memcpy(data ,buffer, sizeof data);
    value.sival_int = toSendPid;
    value.sival_ptr = data;
    sigqueue(toSendPid, SIGUSR1, value); //must be used with same addressspace
}
*/

void Server::readSocketActivity()
{

        if (FD_ISSET(m_sockFd,&m_socksReadFds))
            this->checkNewConnection();
        //for (all entries in queue)
        for(std::vector<unsigned int>::iterator itr= m_connectList.begin(); itr != m_connectList.end(); itr++)
        {
            if (FD_ISSET(*itr,&m_socksReadFds))
                this->checkData(*itr);
        }
}

bool Server::run()
{
	bool ret = false;
    socklen_t clientLen;
    int result = 0;
    struct sockaddr_in client_addr;
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    clientLen = sizeof(client_addr);
    while(1)
    {
        this->buildSelectList();
        this->writeData();
        result = select(m_highSock+1, &m_socksReadFds,&m_socksWriteFds/*(fd_set *) 0*/,
                  (fd_set *) 0,/*NULL*/ &timeout);

        if(result<0)
        {
            cout<<"result<"<<result<<endl;
            break;
        }
        else if(result == 0)
        {

        }
        else
        {
             readSocketActivity();
        }
	}

    close(m_newSockFd);
    close(m_sockFd);
    return ret;
}
