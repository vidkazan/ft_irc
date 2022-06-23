
#include "../main.hpp"

void Chan::setMsgToAllClients(std::string msg, Irc* irc){
    for(std::vector<std::string>::iterator it = _clients.begin();it != _clients.end(); it++){
        irc->findClientByNickName(*it)->allocateResponse(msg);
        irc->findClientByNickName(*it)->setStatus(WRITING);
    }
}