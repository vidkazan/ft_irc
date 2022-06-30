//
// Created by Dmitrii Grigorev on 27.03.22.
//


#include "Client.hpp"
void        Client::generateResponse()
{
    Reply* currentReply;
    std::string bufResp;
    while(_response.getFrontReply()!=nullptr) {
        currentReply = _response.getFrontReply();
        bufResp="";
        switch (currentReply->getCode()) { // TODO change all client hostnames
            case ERR_NORECIPIENT: {
                bufResp = ":"+_irc->getServerName()+" "+" 411 : No recipient given (" + _request.getType() + ")\n";
                break;
            }
            case ERR_CANNOTSENDTOCHAN: {
                bufResp = ":"+_irc->getServerName()+" "+" 404 " + currentReply->getChan() + " :Cannot send to channel\n";
                break;
            }
            case ERR_NOSUCHNICK: {
                bufResp = ":"+_irc->getServerName()+" "+" 401 " + currentReply->getReceiver() + " :No such nick\n";
                break;
            }
            case ERR_NOSUCHCHANNEL: {
                bufResp = ":"+_irc->getServerName()+" "+" 403 " + currentReply->getChan() + " :No such channel\n";
                break;
            }
            case ERR_NOTEXTTOSEND: {
                bufResp = ":"+_irc->getServerName()+" "+" 412 : No text to send\n";
                break;
            }
            case ERR_NEEDMOREPARAMS: {
                bufResp = ":"+_irc->getServerName()+" "+" 461 " + _request.getType() + " :Not enough parameters\n";
                break;
            }
            case ERR_NICKNAMEINUSE: {
                bufResp = ":"+_irc->getServerName()+" "+" 433 " + currentReply->getReceiver() + " :Nickname is already in use\n";
                break;
            }
            case ERR_NOTREGISTERED: {
                bufResp = ":"+_irc->getServerName()+" "+" 451 : You have not registered\n";
                break;
            }
            case ERR_NONICKNAMEGIVEN: {
                bufResp = ":"+_irc->getServerName()+" "+" 431 : No nickname given\n";
                break;
            }
            case ERR_PASSWDMISMATCH: {
                bufResp = ":"+_irc->getServerName()+" "+" 464 : Password incorrect\n";
                break;
            }
            case ERR_ALREADYREGISTRED: {
                bufResp= ":"+_irc->getServerName()+" "+" 462 : You may not register\n";
                break;
            }
            case ERR_NOTONCHANNEL: {
                bufResp = ":"+_irc->getServerName()+" "+" 442 " + currentReply->getChan() + " : You're not on that channel\n";
                break;
            }
            case ERR_UNKNOWNCOMMAND: {
                bufResp = ":"+_irc->getServerName()+" "+" 421 "+_response.getFrontReply()->getOptional(0)+" :Unknown command\n";
                break;
            }
            case ERR_USERNOTINCHANNEL: {
                bufResp = ":"+_irc->getServerName()+" "+" 441 " + currentReply->getReceiver() + " " + currentReply->getChan() + " :They aren't on that channel\n";
                break;
            }
            case ERR_USERONCHANNEL: {
                bufResp = ":"+_irc->getServerName()+" "+" 443 " + currentReply->getReceiver() + " " + currentReply->getReceiver() + " :is already on channel\n";
                break;
            }
            case ERR_INVITEONLYCHAN: {
                bufResp = ":"+_irc->getServerName()+" "+" 473 " + currentReply->getChan() + " :Cannot join channel (+i)\n";
                break;
            }
            case ERR_CHANOPRIVSNEEDED: {
                bufResp = ":"+_irc->getServerName()+" "+" 482 " + _nickname + " :You're not channel operator\n";
                break;
            }
            case ERR_UNKNOWNMODE: {
                bufResp = ":"+_irc->getServerName()+" "+" 472 " + currentReply->getOptional(0) + " :is unknown mode char to me\n";
                break;
            }
            case ERR_BANNEDFROMCHAN: {
                bufResp = ":"+_irc->getServerName()+" "+" 465 "+currentReply->getChan()+" :Cannot join channel (+b)";
                break;
            }
            case RPL_NOTOPIC: {
                bufResp = ":"+_irc->getServerName()+" 331 "+_nickname+" "+currentReply->getChan()+" :No topic is set.\n";
                break;
            }
            case RPL_TOPIC: {
                bufResp = ":"+_irc->getServerName()+" 332 "+_nickname+" "+currentReply->getChan()+" :"+currentReply->getMsg()+"\n";
                break;
            }
            case RPL_LISTSTART: {
                bufResp = ":"+_irc->getServerName()+" 321 Channel :Users  Name\n";
                break;
            }
            case RPL_LIST: {
                for(std::vector<Chan>::iterator it = _irc->getChannels().begin();it != _irc->getChannels().end(); it++){
                    bufResp += ":"+_irc->getServerName()+" 322 "+_nickname+" "+it->getName()+" "+ std::to_string(it->getChanClientsCount()) +" :"+it->getTopic()+"\n";
                }
                break;
            }
            case RPL_LISTEND: {
                bufResp = ":"+_irc->getServerName()+" 323 "+":End of /LIST\n";
                break;
            }
            case RPL_INVITING: {
                bufResp = ":"+_irc->getServerName()+" 341 "+currentReply->getChan()+" "+currentReply->getReceiver()+"\n";
                break;
            }
            case RPL_MOTDSTART:{
                bufResp = ":"+_irc->getServerName()+" 375 " + _nickname + " : Message of the Day -\n";
                break;
            }
            case RPL_MOTD:{
                bufResp += ":"+_irc->getServerName()+" 372 " + _nickname + " :Welcomeeeeeeee!\n";
                break;
            }
            case RPL_ENDOFMOTD:{
                bufResp = ":"+_irc->getServerName()+" 376 " + _nickname + " :End of /MOTD command.\n";
                break;
            }
            case RPL_NAMREPLY:{
                bufResp = ":" + _irc->getServerName() + " 353 " + _nickname + " = " + currentReply->getOptional(0);
                break;
            }
            case RPL_ENDOFNAMES:{
                bufResp = ":" + _irc->getServerName() + " 366 " + currentReply->getChan() + " :End of /NAMES list\n";
                break;
            }
            case ERR_FILEERROR:{
                break;
            }
            case REGISTERED: {
                bufResp = ":"+_irc->getServerName()+" 001 " + _nickname + " :Welcome to the IRC server!\n";
                break;
            }
            case CODE_NOT_SET: {
                break;
            }
            // messages
            case MSG_INVITE: {
//                :pe2!~1@188.234.27.34 INVITE pe1 :#pe
                bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " INVITE " + currentReply->getReceiver() + " :" + currentReply->getChan() +"\n";
                break;
            }
            case MSG_JOIN: {
//                :n22!~2@188.234.27.34 JOIN :#pe
                bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " JOIN :" +currentReply->getChan()+ "\n";
                break;
            }
            case MSG_GROUP_KICK: {
                if(currentReply->getOptional(0).empty())
                    bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " KICK " + currentReply->getChan() + " " + currentReply->getReceiver() + " :"+currentReply->getMsg()+"\n";
                else
                    bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " KICK " + currentReply->getChan() + " " + currentReply->getReceiver() + " :"+currentReply->getSenderNickname()+"\n";
                break;
            }
            case MSG_GROUP_PART: {
//                :n11!~1@188.234.27.34 PART #pe
                bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " PART " + currentReply->getChan() + "\n";
                break;
            }
            case MSG_PRIVMSG: {
                bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " PRIVMSG "+currentReply->getReceiver()+" "+currentReply->getMsg()+"\r\n";
                break;
            }
            case MSG_GROUP_NEWTOPIC: {
                //                :Wiz TOPIC #test :New topic
                bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " TOPIC " + currentReply->getChan() + " :" +
                          currentReply->getMsg() + "\n";
                break;
            }
            case MSG_GROUP_MODE: {
                //   :n22!~2@188.234.27.34 MODE #pe +i
                bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " MODE " + currentReply->getChan()+" "+ currentReply->getMsg() + "\n";
                break;
            }
            case MSG_GROUP_BAN: {
                //                :pepegoB!~1@188.234.27.34 MODE #pe +b pepegoA!*@*
                bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " MODE " + currentReply->getChan()+" "+ currentReply->getMsg() + " "+currentReply->getReceiver()+"\n";
                break;
            }
            case NOTICE_GROUP_INVITE: {
//                :nonstop.ix.me.dal.net NOTICE @#pe :pe2 invited pe1 into channel #pe
                bufResp = ":" +_irc->getServerName()+" NOTICE @" + currentReply->getChan() + " :" + currentReply->getSenderNickname() + " invited " + currentReply->getReceiver()+" into chan "+currentReply->getChan() +"\n";
                break;
            }
            case QUIT_MSG: {
//                n10 [~1@188.234.27.34] has quit IRC: Client closed connection
                bufResp = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname() + " QUIT "+currentReply->getMsg()+"\n";
//                :fcody!~u@188.234.27.34 QUIT :Quit: KVIrc 5.0.0 Aria http://www.kvirc.net/
                break;
            }
        }
        _response.popFrontReply();
        allocateResponse(bufResp);
    }
}
void        Client::allocateResponse(std::string bufResp) {
    std::string newBuf;
    size_t size = 0;
    char *res;
    size_t i=0;
    if(_response.getResponseSize() > 0)
    {
        std::string tmp(_response.getResponse(), _response.getResponseSize());
        newBuf += tmp;
        size += _response.getResponseSize();
    }
    newBuf += bufResp;
    size += bufResp.size();
    res = (char *)malloc(sizeof (char) * (size));
    for(;i < size;i++){
        res[i] = newBuf[i];
    }
    _status = WRITING;
    _response.setResponse(res,i);
}
void        Client::sendResponse()
{
    ssize_t ret;
    ssize_t size;
    if(_response.getResponseSize() - _response.getBytesSent() < 1000000000)
        size = _response.getResponseSize() - _response.getBytesSent();
    else
        size = 999999999;
    if(_response.getResponseSize()) {
        std::string tmp(_response.getResponse(), _response.getResponseSize());
        std::cout << _socketFD << " send:|" << tmp << "|\n";
        ret = send(_socketFD, _response.getResponse() + _response.getBytesSent(), size, 0x80000); //  SIGPIPE ignore
        if (ret <= 0) {
//            std::cout << "send " << _socketFD << ": " << "set to CLOSING" << "  \n";
            free(_response.getResponse());
            Response response;
            setResponse(response);
            Request request;
            _request = request;
            setStatus(CLOSING);
            return;
        }
        _response.addBytesSent(ret);
        std::cout << _socketFD << " send: " << " sent " << _response.getBytesSent() << " respSize "
                  << _response.getResponseSize() << "  \n";
    }
    if(_response.getBytesSent() == (size_t)_response.getResponseSize())
    {
//        std::cout << "send " << _socketFD << ": " << "set to READING" <<  "  \n";
        setStatus(READING);
        free(_response.getResponse());
        Response response;
        setResponse(response);
        Request request;
        _request = request;
    }
}