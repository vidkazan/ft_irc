
#include "../main.hpp"

void Chan::setMsgToAllClients(std::string except,std::string msg, Irc* irc){
    for(std::vector<std::string>::iterator it = _clients.begin();it != _clients.end(); it++){
        if(*it != except) {
            irc->findClientByNickName(*it)->allocateResponse(msg);
            irc->findClientByNickName(*it)->setStatus(WRITING);
        }
    }
}

void Chan::setMsgToAllClients(std::string msg, Irc* irc){
    for(std::vector<std::string>::iterator it = _clients.begin();it != _clients.end(); it++){
            irc->findClientByNickName(*it)->allocateResponse(msg);
            irc->findClientByNickName(*it)->setStatus(WRITING);
    }
}