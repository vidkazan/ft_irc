#include "Chan.hpp"

void Chan::setMsgToAllClients(Irc* irc, Reply msg,std::string except){
    for(std::map<std::string,t_user_modes>::iterator it = _clients.begin();it != _clients.end(); it++){
        if(it->first != except && irc->findClientByNickName(it->first)) {
            irc->findClientByNickName(it->first)->getResponse().addMsg(msg);
            irc->findClientByNickName(it->first)->generateResponse();
        }
    }
}