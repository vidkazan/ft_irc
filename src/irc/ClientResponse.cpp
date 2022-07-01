//
// Created by Dmitrii Grigorev on 27.03.22.
//


#include "Client.hpp"
void        Client::generateResponse()
{
    Reply* currentReply;
    std::string serverPrefix;
    std::string hostPrefix;
    std::string bufResp;
    while(_response.getFrontReply()!=_response.getEnd()) {
        currentReply = _response.getFrontReply().operator->();
        serverPrefix = ":"+_irc->getServerName();
        hostPrefix = ":" + currentReply->getSenderNickname() + "!~"+ currentReply->getSenderUsername() + "@" + currentReply->getSenderHostname();
        bufResp="";
        switch (currentReply->getCode()) {
            case ERR_NORECIPIENT: {
                bufResp = serverPrefix+" "+" 411 : No recipient given (" + _request.getType() + ")\n";
                break;
            }
            case ERR_CANNOTSENDTOCHAN: {
                bufResp = serverPrefix+" "+" 404 " + currentReply->getChan() + " :Cannot send to channel\n";
                break;
            }
            case ERR_NOSUCHNICK: {
                bufResp = serverPrefix+" "+" 401 " + currentReply->getReceiver() + " :No such nick\n";
                break;
            }
            case ERR_NOSUCHCHANNEL: {
                bufResp = serverPrefix+" "+" 403 " + currentReply->getChan() + " :No such channel\n";
                break;
            }
            case ERR_NOTEXTTOSEND: {
                bufResp = serverPrefix+" "+" 412 : No text to send\n";
                break;
            }
            case ERR_NEEDMOREPARAMS: {
                bufResp = serverPrefix+" "+" 461 " + _request.getType() + " :Not enough parameters\n";
                break;
            }
            case ERR_NICKNAMEINUSE: {
                bufResp = serverPrefix+" "+" 433 " + currentReply->getReceiver() + " :Nickname is already in use\n";
                break;
            }
            case ERR_NOTREGISTERED: {
                bufResp = serverPrefix+" "+" 451 : You have not registered\n";
                break;
            }
            case ERR_NONICKNAMEGIVEN: {
                bufResp = serverPrefix+" "+" 431 : No nickname given\n";
                break;
            }
            case ERR_PASSWDMISMATCH: {
                bufResp = serverPrefix+" "+" 464 : Password incorrect\n";
                break;
            }
            case ERR_ALREADYREGISTRED: {
                bufResp= serverPrefix+" "+" 462 : You may not register\n";
                break;
            }
            case ERR_NOTONCHANNEL: {
                bufResp = serverPrefix+" "+" 442 " + currentReply->getChan() + " : You're not on that channel\n";
                break;
            }
            case ERR_UNKNOWNCOMMAND: {
                bufResp = serverPrefix+" "+" 421 "+_response.getFrontReply()->getOptional(0)+" :Unknown command\n";
                break;
            }
            case ERR_USERNOTINCHANNEL: {
                bufResp = serverPrefix+" "+" 441 " + currentReply->getReceiver() + " " + currentReply->getChan() + " :They aren't on that channel\n";
                break;
            }
            case ERR_USERONCHANNEL: {
                bufResp = serverPrefix+" "+" 443 " + currentReply->getReceiver() + " " + currentReply->getReceiver() + " :is already on channel\n";
                break;
            }
            case ERR_INVITEONLYCHAN: {
                bufResp = serverPrefix+" "+" 473 " + currentReply->getChan() + " :Cannot join channel (+i)\n";
                break;
            }
            case ERR_CHANOPRIVSNEEDED: {
                bufResp = serverPrefix+" "+" 482 " + _nickname + " :You're not channel operator\n";
                break;
            }
            case ERR_ERRONEUSNICKNAME: {
//                :nonstop.ix.me.dal.net 432 d f :Erroneous Nickname
                bufResp = serverPrefix+" "+" 432 " + currentReply->getMsg() + " :Erroneous Nickname\n";
                break;
            }
            case ERR_UNKNOWNMODE: {
                bufResp = serverPrefix+" "+" 472 " + currentReply->getOptional(0) + " :is unknown mode char to me\n";
                break;
            }
            case ERR_BANNEDFROMCHAN: {
                bufResp = serverPrefix+" "+" 465 "+currentReply->getChan()+" :Cannot join channel (+b)\n";
                break;
            }
            case RPL_NOTOPIC: {
                bufResp = serverPrefix+" 331 "+_nickname+" "+currentReply->getChan()+" :No topic is set.\n";
                break;
            }
            case RPL_TOPIC: {
                bufResp = serverPrefix+" 332 "+_nickname+" "+currentReply->getChan()+" :"+currentReply->getMsg()+"\n";
                break;
            }
            case RPL_LISTSTART: {
                bufResp = serverPrefix+" 321 Channel :Users  Name\n";
                break;
            }
            case RPL_LIST: {
                for(std::vector<Chan>::iterator it = _irc->getChannels().begin();it != _irc->getChannels().end(); it++){
                    bufResp += serverPrefix+" 322 "+_nickname+" "+it->getName()+" "+ std::to_string(it->getChanClientsCount()) +" :"+it->getTopic()+"\n";
                }
                break;
            }
            case RPL_LISTEND: {
                bufResp = serverPrefix+" 323 "+":End of /LIST\n";
                break;
            }
            case RPL_INVITING: {
                bufResp = serverPrefix+" 341 "+currentReply->getChan()+" "+currentReply->getReceiver()+"\n";
                break;
            }
            case RPL_MOTDSTART:{
                bufResp = serverPrefix+" 375 " + _nickname + " : Message of the Day -\n";
                break;
            }
            case RPL_MOTD:{
                bufResp += serverPrefix+" 372 " + _nickname + " :Welcomeeeeeeee!\n";
                break;
            }
            case RPL_ENDOFMOTD:{
                bufResp = serverPrefix+" 376 " + _nickname + " :End of /MOTD command.\n";
                break;
            }
            case RPL_NAMREPLY:{
                bufResp = ":" + _irc->getServerName() + " 353 " + _nickname + " = " + currentReply->getOptional(0)+"\n";
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
                bufResp = serverPrefix+" 001 " + _nickname + " :Welcome to the IRC server!\n";
                break;
            }
            case CODE_NOT_SET: {
                break;
            }
            // messages
            case MSG_INVITE: {
//                :pe2!~1@188.234.27.34 INVITE pe1 :#pe
                bufResp = hostPrefix + " INVITE " + currentReply->getReceiver() + " :" + currentReply->getChan() +"\n";
                break;
            }
            case MSG_JOIN: {
//                :n22!~2@188.234.27.34 JOIN :#pe
                bufResp = hostPrefix + " JOIN :" +currentReply->getChan()+ "\n";
                break;
            }
            case MSG_GROUP_KICK: {
                if(currentReply->getOptional(0).empty())
                    bufResp = hostPrefix + " KICK " + currentReply->getChan() + " " + currentReply->getReceiver() + " :"+currentReply->getMsg()+"\n";
                else
                    bufResp = hostPrefix + " KICK " + currentReply->getChan() + " " + currentReply->getReceiver() + " :"+currentReply->getSenderNickname()+"\n";
                break;
            }
            case MSG_GROUP_PART: {
//                :n11!~1@188.234.27.34 PART #pe
                bufResp = hostPrefix + " PART " + currentReply->getChan() + "\n";
                break;
            }
            case MSG_PRIVMSG: {
                bufResp = hostPrefix + " PRIVMSG "+currentReply->getReceiver()+" "+currentReply->getMsg()+"\r\n";
                break;
            }
            case MSG_NOTICE: {
                bufResp = hostPrefix + " NOTICE "+currentReply->getReceiver()+" "+currentReply->getMsg()+"\r\n";
                break;
            }
            case MSG_GROUP_NEWTOPIC: {
                //                :Wiz TOPIC #test :New topic
                bufResp = hostPrefix + " TOPIC " + currentReply->getChan() + " :" +
                          currentReply->getMsg() + "\n";
                break;
            }
            case MSG_GROUP_MODE: {
                //   :n22!~2@188.234.27.34 MODE #pe +i
                bufResp = hostPrefix + " MODE " + currentReply->getChan()+" "+ currentReply->getMsg() + "\n";
                break;
            }
            case MSG_GROUP_OPER:
            case MSG_GROUP_EDIT_MODE: {
                //                :pepegoB!~1@188.234.27.34 MODE #pe +b pepegoA!*@*
                bufResp = hostPrefix + " MODE " + currentReply->getChan()+" "+ currentReply->getMsg() + " "+currentReply->getReceiver()+"\n";
                break;
            }
            case NOTICE_GROUP_INVITE: {
//                :nonstop.ix.me.dal.net NOTICE @#pe :pe2 invited pe1 into channel #pe
                bufResp = serverPrefix +" NOTICE @" + currentReply->getChan() + " :" + currentReply->getSenderNickname() + " invited " + currentReply->getReceiver()+" into chan "+currentReply->getChan() +"\n";
                break;
            }
            case QUIT_MSG: {
//                n10 [~1@188.234.27.34] has quit IRC: Client closed connection
                bufResp = hostPrefix + " QUIT "+currentReply->getMsg()+"\n";
//                :fcody!~u@188.234.27.34 QUIT :Quit: KVIrc 5.0.0 Aria http://www.kvirc.net/
                break;
            }
        }
        _lastCode = currentReply->getCode();
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
        std::cout << _socketFD << " send: " << tmp << "\n";
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
//        std::cout << _socketFD << " send: " << " sent " << _response.getBytesSent() << " respSize "
//                  << _response.getResponseSize() << "  \n";
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