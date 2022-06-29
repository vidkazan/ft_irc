#pragma once
#include "../irc/Chan.hpp"
#include "../irc/Client.hpp"
#include "PortServer.hpp"

class Chan;
class Client;
class Irc{
private:
	PortServer                  _portServer;
	std::vector<Client>         _clients;
    std::string                 _password;
    std::vector<Chan>        _channels;
    std::string                 _serverName;
public:
    Irc(int port, std::string ip, std::string pass, std::string name = "localhost"): _portServer(port,ip), _password(pass), _serverName(name)
    {
        if(bind(_portServer.getSocketFD(), (struct sockaddr *)&_portServer.getSockAddrInStruct(), sizeof(_portServer.getSockAddrInStruct())) < 0){
            perror("bind error");
            exit(EXIT_FAILURE);
        }
    }
    ~Irc(){}
    Client*                     findClientByNickName(std::string nickname);
    Chan*                     findChanByName(std::string name);
	PortServer&    getPortServer();
	std::vector<Client>&        getClients();
    std::string       getServerName();
    std::vector<Chan>&        getChannels();
    bool                        checkPassword(std::string check);
	void                        addClient(int fd, Irc* irc,std::string ip);
    bool                        addChannel(std::string name, std::string nickname);
    void                     printAllClients();
    void                    setMsgToClient(std::string senderNickname, std::string nickname, std::string msg);
};

