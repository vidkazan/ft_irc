#pragma once
#include "../main.hpp"

class PortServer;

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
    Client*                     findClientByNickName(std::string nickname)
    {
        for(std::vector<Client>::iterator it = getClients().begin();it != getClients().end(); it++){
//            std::cout << "findClientByNickName: check: |" << it->getNickName() << "|" << nickname << "|\n";
            if(it->getNickName() == nickname)
            {
                return(&(*it));
            }
        }
        std::cout << "findClientByNickName: not found\n";
        return(nullptr);
    }
    Chan*                     findChanByName(std::string name)
    {
        for(std::vector<Chan>::iterator it = getChannels().begin();it != getChannels().end(); it++){
//            std::cout << "findClientByNickName: check: |" << it->getNickName() << "|" << nickname << "|\n";
            if(it->getName() == name)
            {
                return(&(*it));
            }
        }
        std::cout << "findChanByName: not found\n";
        return(nullptr);
    }
	void                        configFileParse(){}
	PortServer&    getPortServer(){return _portServer;}
	std::vector<Client>&        getClients(){return _clients;}
    std::string       getServerName(){return _serverName;}
    std::vector<Chan>&        getChannels(){return _channels;}
    bool                        checkPassword(std::string check)
    {
        if(check == _password)
            return true;
        else
            return false;
    }
	void                        addClient(int fd, Irc* irc)
    {
		Client client(fd, irc);
		_clients.push_back(client);
	}
    bool                        addChannel(std::string name, std::string nickname)
    {
        for(std::vector<Chan>::iterator it = _channels.begin();it != _channels.end(); it++){
            if(it->getName() == name)
            {
                if(it->addClient(nickname))
                    return 1;
                else
                    return 0; // already there
            }
        }
        std::cout << "irc: create new chan: " << name << " by " << nickname << "\n";
        Chan chan(nickname,name);
        _channels.push_back(chan);
        _channels.back().addClient(nickname);
        return 1;
    }
    void                     printAllClients()
    {
        for(std::vector<Client>::iterator it = getClients().begin();it != getClients().end(); it++){
            std::cout << "Client " << it->getSocketFd() << " " << it->getNickName() << "\n";
        }
    }
    std::string              channelsList(std::string nickname)
    {
        std::string buf;
//        :irc.colosolutions.net 321 pepego1 Channel :Users  Name
        buf +=":"+_serverName+" 321 Channel :Users  Name\n";
        for(std::vector<Chan>::iterator it = getChannels().begin();it != getChannels().end(); it++){
            std::cout << _serverName << " " << it->getName() << "\n";
            buf += ":"+_serverName+" 322 "+nickname+" "+it->getName()+" "+ std::to_string(it->getChanClientsCount()) +" :"+it->getTopic()+"\n";
        }
        buf += ":"+_serverName+" 323 "+":End of /LIST\n";
        return buf;
    }
    void                    setMsgToClient(std::string senderNickname, std::string nickname, std::string msg){
        if(findClientByNickName(nickname))
            findClientByNickName(nickname)->allocateResponse(msg);
    }
};

