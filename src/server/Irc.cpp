#include "Irc.hpp"

Client*               Irc::findClientByNickName(std::string nickname)
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
Chan*                 Irc::findChanByName(std::string name)
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
PortServer&           Irc::getPortServer(){return _portServer;}
std::vector<Client>&  Irc::getClients(){return _clients;}
std::string           Irc::getServerName(){return _serverName;}
std::vector<Chan>&    Irc::getChannels(){return _channels;}
bool                  Irc::checkPassword(std::string check)
{
    if(check == _password)
        return true;
    else
        return false;
}
void                  Irc::addClient(int fd, Irc* irc)
{
    Client client(fd, irc);
    _clients.push_back(client);
}
bool                  Irc::addChannel(std::string name, std::string nickname)
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
void                  Irc::printAllClients()
{
    for(std::vector<Client>::iterator it = getClients().begin();it != getClients().end(); it++){
        std::cout << "Client " << it->getSocketFd() << " " << it->getNickName() << "\n";
    }
}
void                  Irc::setMsgToClient(std::string senderNickname, std::string nickname, std::string msg){
    if(findClientByNickName(nickname))
        findClientByNickName(nickname)->allocateResponse(msg);
}