//#include <csignal>
#include <sys/signal.h>
#include "ServerHandler.h"
#include <thread>

using namespace std;
//for constant we can init the below inside
//the class declaration
bool ServerHandler::readSuccessful = false;
ServerHandler::ServerHandler(Server * chatServer):m_chatServer(chatServer)
{

    signal(SIGUSR1, ServerHandler::onData);

}

ServerHandler::~ServerHandler()
{
}

//define handler here
void ServerHandler::onData(int signal)
{
    cout<<"inside usr1 handler"<<endl;
    //got the data manipulate it m_chatServer->m_readData
    //send the data to all clients
    ServerHandler::readSuccessful = true;
    //raise(SIGUSR2);
}

void ServerHandler::init()
{
  //  std::thread first(this->initServer);
    while(1)
    {
        if(ServerHandler::readSuccessful)
        {
            ServerHandler::readSuccessful = false;

            cout<<"readSuccessfull"<<endl;
            for(std::map<unsigned int , ClientInfo*>::iterator itr=m_chatServer->m_clientMap.begin();
                itr!=m_chatServer->m_clientMap.end();
                itr++)
            {
                switch(itr->second->getState())
                {
                    case clientState::getName:
                        {

                        std::string  name=itr->second->m_data;
                        std::cout<<"getNAme    "<<name<<endl;
                        itr->second->setName(name);
                        itr->second->setState(clientState::idle);
                        break;
                        }
                    case clientState::readable:
                        {

                        std::string temp = itr->second->getName() +"-->"+ itr->second->m_data;
                        itr->second->m_dataToWrite = temp;
                        itr->second->setState(clientState::writable);
                        raise(SIGUSR2);
                        break;
                        }

                }
                //break;
            }
            //raise(SIGUSR2);
        }

        usleep(200*1000);
    }
}
