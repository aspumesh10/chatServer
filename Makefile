CXXFLAGS=-std=c++11
chatServer:chatServerBin
chatServerBin:main.o Server.o ServerHandler.o ClientInfo.o
	g++ -pthread main.o Server.o ServerHandler.o ClientInfo.o -o chatServerBin
main.o:main.cpp
	g++ $(CXXFLAGS) main.cpp -c
Server.o:Server.cpp
	g++  $(CXXFLAGS) Server.cpp -c
CLientInfo.o:CLientInfo.cpp
	g++  $(CXXFLAGS) CLientInfo.cpp -c
ServerHandler.o:ServerHandler.cpp
	g++  $(CXXFLAGS) ServerHandler.cpp -c        
clean:
	rm -f  *.o  *.out
run:chatServerBin 
	./chatServerBin
