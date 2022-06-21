#pragma once
#include "../main.hpp"

class PortServer;

class Irc{
private:
	PortServer                  _portServer;
	std::vector<Client>         _clients;
    std::string                 _password;
public:
	                            Irc(int port, std::string ip, std::string pass): _portServer(port,ip), _password(pass){
                                    if(bind(_portServer.getSocketFD(), (struct sockaddr *)&_portServer.getSockAddrInStruct(), sizeof(_portServer.getSockAddrInStruct())) < 0){
                                        perror("bind error");
                                        exit(EXIT_FAILURE);
                                    }
                                }
	                            ~Irc(){}
	void                        configFileParse(){}
	PortServer&    getPortServer(){return _portServer;}
	std::vector<Client>&        getClients(){return _clients;}
    bool                        checkPassword(std::string check){
                                    return(check == _password);
                                }
	void                        addClient(int fd)
    {
		Client client(fd);
		_clients.push_back(client);
	}

};

