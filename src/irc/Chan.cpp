
#include "../main.hpp"

void Chan::setMsgToAllClients(std::string except,std::string msg, Irc* irc){
    for(std::vector<std::pair<std::string,t_user_modes> >::iterator it = _clients.begin();it != _clients.end(); it++){
        if(it->first != except && irc->findClientByNickName(it->first)) {
            irc->findClientByNickName(it->first)->allocateResponse(msg);
            irc->findClientByNickName(it->first)->setStatus(WRITING);
        }
    }
}

void Chan::setMsgToAllClients(std::string msg, Irc* irc){
    for(std::vector<std::pair<std::string,t_user_modes> >::iterator it = _clients.begin();it != _clients.end(); it++){
        if( irc->findClientByNickName(it->first)) {
            irc->findClientByNickName(it->first)->allocateResponse(msg);
            irc->findClientByNickName(it->first)->setStatus(WRITING);
        }
    }
}