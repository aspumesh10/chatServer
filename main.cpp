#include <iostream>
#include <thread>
#include "ServerHandler.h"
using namespace std;
void ServerThread(Server * chatServer)
{
    //Server * chatServer = new Server(4000);
    if(!chatServer->createServer())
    {

        cout<<"server Creation Success"<<endl;
    }
    else if(!chatServer->run())
    {

        cout<<"server broke in running"<<endl;
    }
    else
    {
        cout<<"server closed success"<<endl;
    }
    delete chatServer;
    exit(0);
}


int main()
{
    Server * chatServer = new Server(4000);
    std::thread serverTd(ServerThread, chatServer);

    ServerHandler* customServerHandler = new ServerHandler(chatServer);
    customServerHandler->init();

    cout<<"main::Server stopped"<<endl;
    delete customServerHandler;
    return 0;
}
