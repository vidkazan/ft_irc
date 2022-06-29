//
// Created by Dmitrii Grigorev on 27.03.22.
//
#include "Client.hpp"
#include "../main.hpp"

        Client::Client(int fd, Irc* irc): _socketFD(fd),\
                                        _status(READING),\
                                        _isAuthorisedPass(0),\
                                        _isAuthorisedNickUser(0),\
                                        _irc(irc) {
                                        }

void        Client::analyseRequest(std::string line)
{
    std::string tmp = line;
    std::vector<std::string> attributes;
    size_t pos = line.find(' ');
    if(pos != std::string::npos) {
        line.erase(0,pos + 1);
    }
    else if (_request.getRequestMethod() != NO_METHOD && _request.getRequestMethod() != OTHER_METHOD) {
        line = "";
    }
//    std::cout << "line erased:|" << line << "|\n";
    switch (_request.getRequestMethod()) {
        case TOPIC: {
                methodTopic(line);
                break;
        }
        case NO_METHOD: {
            _response.addReply(CODE_NOT_SET);
            break;
        }
        case OTHER_METHOD: {
            if (_isAuthorisedNickUser) {
                _response.addReply(ERR_UNKNOWNCOMMAND);
                _response.getLastReply()->addOptional(tmp);
            }
            break;
        }
        case QUIT: {
            _status = CLOSING;
            break;
        }
        case PASS: {
            methodPass(line);
            break;
        }
        case NICK: {
            methodNick(line);
            break;
        }
        case USER: {
            methodUser(line);
            break;
        }
        case NOTICE:
        case PRIVMSG: {
            methodPrivmsg(line);
            break;
        }
        case INVITE: {
            methodInvite(line);
            break;
        }
        case JOIN: {
            methodJoin(line);
            break;
        }
        case PING: {
            methodPing(line);
            break;
        }
        case NAMES: {
            if (!_isAuthorisedNickUser) {
                _response.addReply(ERR_NOTREGISTERED);
            } else if (line.empty()) {
                _response.addReply(ERR_NEEDMOREPARAMS);
            } else if (!_irc->findChanByName(line)) {
                _response.addReply(ERR_NOSUCHCHANNEL,line);
            } else {
                methodNames(line);
            }
            break;
        }
        case LIST: {
            if (!_isAuthorisedNickUser) {
                _response.addReply(ERR_NOTREGISTERED);
            } else {
                _response.addReply(RPL_LISTSTART);
                _response.addReply(RPL_LIST);
                _response.addReply(RPL_LISTEND);
            }
            break;
        }
        case KICK: {
            methodKick(line);
            break;
        }
        case PART:
        {
            methodPart(line);
            break;
        }
        case MODE: {
            methodMode(line);
            break;
        }
        case USERHOST: {
            break;
        }
        case WHO: {
            methodWho(line);
//            :punch.wa.us.dal.net 352 pepenick #ch ~u 188.234.27.34 punch.wa.us.dal.net pepenick H@ :0 r
            break;
        }
    }
    generateResponse();
}

bool       Client::checkPassword(std::string p)
{
    return(_irc->checkPassword(p));
}
void       Client::methodNames(std::string line)
{
    _response.addReply(RPL_NAMREPLY,line);
    _response.getLastReply()->addOptional(_irc->findChanByName(line)->getChanNames());
    _response.addReply(RPL_ENDOFNAMES,line);
}
void       Client::methodTopic(std::string line)
{
    size_t pos;
    bool setTopic = 0;
    std::string chan;
    std::string topic;
    if (!_isAuthorisedNickUser) {
        _response.addReply(ERR_NOTREGISTERED);
    } else {
        pos = line.find(':');
        if (pos != std::string::npos) {
            setTopic = 1;
            chan = line.substr(0, pos - 1);
            if (line.size() - pos > 1)
                topic = line.substr(pos + 1, line.size() - pos - 1);
        } else {
            chan = line;
        }
        std::cout << ">>>TOPIC: |" << chan << "|" << topic << "|\n";
        if(chan.empty()) {
            _response.addReply(ERR_NEEDMOREPARAMS);
        } else if (!_irc->findChanByName(chan)) {
            _response.addReply(ERR_NOSUCHCHANNEL,chan);
        } else if (_irc->findChanByName(chan)->checkChanClientBanned(_nickname)){
            _response.addReply(ERR_BANNEDFROMCHAN,chan);
        } else if (!_irc->findChanByName(chan)->findClient(_nickname)){
            _response.addReply(ERR_NOTONCHANNEL,chan);
        } else {
            if(setTopic) {
                _irc->findChanByName(chan)->setTopic(topic);
                std::cout << ">>>:"+_nickname+" TOPIC add"+chan+" :"+topic+"\n"; // TODO
                return;
            } else {
                topic = _irc->findChanByName(chan)->getTopic();
                std::cout << ">>>:"+_nickname+" TOPIC read"+chan+" :"+topic+"\n"; // TODO
                if(topic.empty()) {
                    _response.addReply(RPL_NOTOPIC, chan);
                } else {
                    _response.addReply(RPL_TOPIC, chan,"","",topic);
                }
            }
        }
    }
}
void       Client::methodPass(std::string line){
    if (line[0] == ':')
        line.erase(line.begin());
    if (_isAuthorisedPass) {
        std::cout << _socketFD << " PASS ERR_ALREADYREGISTRED \n";
        _response.addReply(ERR_ALREADYREGISTRED);
    } else if (line.empty()) {
        std::cout << _socketFD << " PASS ERR_NEEDMOREPARAMS \n";
        _response.addReply(ERR_NEEDMOREPARAMS);
    } else if (checkPassword(line)) {
        _isAuthorisedPass = 1;
        std::cout << _socketFD << " PASS CORRECT \n";
    } else {
        std::cout << _socketFD << " PASS ERR_PASSWDMISMATCH \n";
        _response.addReply(ERR_PASSWDMISMATCH);
    }
}
void       Client::methodNick(std::string line){
    if (_isAuthorisedNickUser) {
        std::cout << _socketFD << "NICK ERR_ALREADYREGISTRED \n";
        _response.addReply(ERR_ALREADYREGISTRED);
    } else if (!_isAuthorisedPass) {
        std::cout << _socketFD << "NICK ERR_NOTREGISTERED \n";
        _response.addReply(ERR_NOTREGISTERED);
    } else if (line.empty()) {
        std::cout << _socketFD << "NICK ERR_NONICKNAMEGIVEN \n";
        _response.addReply(ERR_NONICKNAMEGIVEN);
    } else if (_irc->findClientByNickName(line)) {
        std::cout << _socketFD << "NICK ERR_NICKNAMEINUSE \n";
        _response.addReply(ERR_NICKNAMEINUSE,"",line);
    } else {
        std::cout << "NICKNAME is SET: " << line << " \n";
        _nickname = line;
        if(!_nickname.empty() && !_username.empty() && !_hostname.empty() && !_servername.empty() && !_realname.empty()) {
            _isAuthorisedNickUser = true;
            _response.addReply(REGISTERED);
            _response.addReply(RPL_MOTDSTART);
            _response.addReply(RPL_MOTD);
            _response.addReply(RPL_ENDOFMOTD);
        } else {
            return;
        }
    }
}
void       Client::methodUser(std::string line){
    size_t pos;
    std::string us,ho,se,re;
    if (_isAuthorisedNickUser) {
        std::cout << _socketFD << "NICK ERR_ALREADYREGISTRED \n";
        _response.addReply(ERR_ALREADYREGISTRED);
    } else if (!_isAuthorisedPass) {
        std::cout << _socketFD << "NICK ERR_NOTREGISTERED \n";
        _response.addReply(ERR_NOTREGISTERED);
    } else {
        pos = line.find(' ');
        if(pos!= std::string::npos){
            us = line.substr(0,pos);
            line.erase(0,pos + 1);
        }
        pos = line.find(' ');
        if(pos!= std::string::npos){
            ho = line.substr(0,pos);
            line.erase(0,pos+1);
        }
        pos = line.find(' ');
        if(pos!= std::string::npos){
            se = line.substr(0,pos);
            line.erase(0,pos+1);
        }
        re = line;
        if (us.empty() || ho.empty() || se.empty() || re.empty()) {
            std::cout << _socketFD << "NICK ERR_NEEDMOREPARAMS \n";
            _response.addReply(ERR_NEEDMOREPARAMS);
        } else {
            _username = us;
            _hostname = ho;
            _servername = se;
            _realname = re;
            if(!_nickname.empty() && !_username.empty() && !_hostname.empty() && !_servername.empty() && !_realname.empty()) {
                _isAuthorisedNickUser = true;
                _response.addReply(REGISTERED);
                _response.addReply(RPL_MOTDSTART);
                _response.addReply(RPL_MOTD);
                _response.addReply(RPL_ENDOFMOTD);
            } else {
                return;
            }
        }
    }
}

// usage: <receiver>{,<receiver>} <text to be sent>
void       Client::methodPrivmsg(std::string line){
    if (!_isAuthorisedNickUser) {
        _response.addReply(ERR_NOTREGISTERED);
    } else if (line.empty()) {
        _response.addReply(ERR_NORECIPIENT);
    } else {

        /*
        посчитать кол-во запятых, выделить память под это кол-во (?? может ли быть запятая в нике ??);
        разделить receiverNames по запятым и засунуть в массив names;
        for по этим именам;
        */


        // находим позицию :
        size_t pos = line.find(':');
        if (pos == std::string::npos) {
            _response.addReply(ERR_NOTEXTTOSEND);
            return;
        }

        // отправляем сроку только с именами и запятыми
        std::vector<std::string> names = split(line.substr(0, pos - 1), ",");
        int n = names.size();

        for (int i = 0; i < n; i++) {
            std::cout << "--- " << i << " " << names[i] << "\n";
            
    //                std::cout << "PRIVMSG: buffer |" << line << "|\n";
            std::string msg;
            std::string receiverName;
            receiverName = names[i];
            // size_t pos = line.find(':');
            // if (pos != std::string::npos) {
            // receiverName = line.substr(0, pos - 1);
                //    std::cout << "analyse: PRIVMSG: receiverName " << receiverName << "\n";
            msg = ":" + _nickname + " PRIVMSG " + receiverName + " " +
                line.substr(pos, _request.getBuffer().size() - pos);
//                    std::cout << "analyse: PRIVMSG: msg " << msg << "\n";
            msg.append("\r\n");
            if (receiverName[0] == '#' || receiverName[0] == '&') {
                if (_irc->findChanByName(receiverName) == nullptr) {
                    _response.addReply(ERR_NOSUCHCHANNEL,receiverName);
                } else if (_irc->findChanByName(receiverName)->checkChanClientBanned(_nickname)){
                    _response.addReply(ERR_BANNEDFROMCHAN,receiverName);
                } else if (!_irc->findChanByName(receiverName)->findClient(_nickname)) {
                    _response.addReply(ERR_CANNOTSENDTOCHAN,receiverName);
                } else {
                    _irc->findChanByName(receiverName)->setMsgToAllClients(_nickname,msg, _irc);
                    // return;
                    
                    continue;
                }
            } else {
                if (_irc->findClientByNickName(receiverName) != nullptr) {
                    _irc->findClientByNickName(receiverName)->allocateResponse(msg);
                    _irc->findClientByNickName(receiverName)->setStatus(WRITING);
                    // return;
                    continue;
                } else {
                    std::cout << "&&&&&&&&& " << i << " " << receiverName << "\n";
                    _response.addReply(ERR_NOSUCHNICK,"",receiverName);
                }
            }
            // }
            // else {
            //     _response.addReply(ERR_NOTEXTTOSEND);
            // }
        }

        // delete[] names;
    }
}
void       Client::methodMode(std::string line){ // TODO messages check
    char modeChar = 0;
    int modeSign = -1;
    size_t pos1;
    size_t pos2;
    std::string chan;
    std::string flags;
    std::string client;
    if (!_isAuthorisedNickUser) {
        _response.addReply(ERR_NOTREGISTERED);
    } else if (line.empty()) {
        _response.addReply(ERR_NEEDMOREPARAMS);
    } else {
//                <channel> {[+|-]|i|t|b} [<user>]
        pos1 = line.find_first_of(' ');
        pos2 = line.find_last_of(' ');
        if(pos1!=std::string::npos) {
            chan = line.substr(0,pos1);
            if(pos2!=std::string::npos && pos1 != pos2) {
                client = line.substr(pos2+1, line.size()-pos2-1);
                flags= line.substr(pos1+1, pos2-pos1-1);
            } else {
                flags= line.substr(pos1+1, line.size()-pos1-1);
            }
            if(flags[0]=='+')
                modeSign = 1;
            else if(flags[0]=='-')
                modeSign = 0;
            else
                modeSign = -1;
            modeChar = flags[1];
        }
        std::cout << "MODE: |"<<chan<<"|"<<flags<<"|"<<client<<"|\n";
        if(chan.empty() || flags.empty()) {
            _response.addReply(ERR_NEEDMOREPARAMS);
        } else if(!_irc->findChanByName(chan)) {
            _response.addReply(ERR_NOSUCHCHANNEL,chan);
        } else if (_irc->findChanByName(chan) && _irc->findChanByName(chan)->checkChanClientBanned(_nickname)){
            _response.addReply(ERR_BANNEDFROMCHAN,chan);
        } else if(!_irc->findChanByName(chan)->findOperator(_nickname)) {
            _response.addReply(ERR_CHANOPRIVSNEEDED,chan);
        } else if(!client.empty() &&!_irc->findChanByName(chan)->findClient(client)) {
            _response.addReply(ERR_USERNOTINCHANNEL,chan,client);
        } else if(modeChar!='i' && modeChar!='b' && modeChar!='t' || modeSign<0 || flags.size() != 2){
            _response.addReply(ERR_UNKNOWNMODE);
            _response.getLastReply()->addOptional(flags);
        } else {
            if(client.empty())
                _irc->findChanByName(chan)->setChanInviteMode(modeChar, modeSign);
            else {
                if (flags == "+b") {
                    _irc->findChanByName(chan)->addClientBan(client);
                    _irc->findChanByName(chan)->removeClient(client);
                }
                if (flags == "-b") {
                    _irc->findChanByName(chan)->deleteClientBan(client);
                }
            }
            std::cout << "MODE: success\n";
            return;
        }
    }
}
void       Client::methodPart(std::string line){
    size_t pos = line.find(':');
    if(pos != std::string::npos){
        line.erase(pos-1, line.size() - pos +1);
    }
//            std::cout << "ch:|" << line << "|\n";
    if (!_isAuthorisedNickUser) {
        _response.addReply(ERR_NOTREGISTERED);
    } else if(line.empty()) {
        _response.addReply(ERR_NEEDMOREPARAMS);
    } else if (!_irc->findChanByName(line)){
        _response.addReply(ERR_NOSUCHCHANNEL,line);
    } else if (!_irc->findChanByName(line)->findClient(_nickname)) {
        _response.addReply(ERR_NOTONCHANNEL,line);
    } else {
//                :P11!~1@89.232.114.120 PART #pp
        std::string msg = ":" + _nickname + " PART :" + line + "\n";
        _irc->findChanByName(line)->setMsgToAllClients(msg, _irc);
        _irc->findChanByName(line)->removeClient(_nickname);
    }
}
void       Client::methodKick(std::string line){
    size_t pos, pos1;
    std::string chan;
    std::string client;
    std::string buf;
    std::string msg;
    if (!_isAuthorisedNickUser) {
        _response.addReply(ERR_NOTREGISTERED);
    } else {
        pos = line.find(' ');
        pos1 = line.find_last_of(':');
        if (pos!=std::string::npos) {
            chan = line.substr(0,pos);
            if (pos1!=std::string::npos) {
                if (pos1 > pos && line.size() > pos1) {
                    msg = line.substr(pos1 + 1, line.size() - pos1);
                    client = line.substr(pos+1, pos1-pos -1);
                }
            } else {
                client = line.substr(pos+1, line.size() - pos -1);
            }
        }
        std::cout << "KICK: |"<<chan<<"|"<<client<<"|\n";
        if(chan.empty() || client.empty()) {
            _response.addReply(ERR_NEEDMOREPARAMS);
        } else if(!_irc->findChanByName(chan)) {
            _response.addReply(ERR_NOSUCHCHANNEL,chan);
        } else if (_irc->findChanByName(chan) && _irc->findChanByName(chan)->checkChanClientBanned(_nickname)){
            _response.addReply(ERR_BANNEDFROMCHAN,chan);
        } else if(!_irc->findChanByName(chan)->findOperator(_nickname)) {
            _response.addReply(ERR_CHANOPRIVSNEEDED,chan);
        } else if(!_irc->findChanByName(chan)->findClient(client)) {
            _response.addReply(ERR_USERNOTINCHANNEL,chan,client);
        } else {
            if(msg.empty())
                buf = ":" + _nickname + " KICK " + chan + " " + client + " :"+msg +"\n";
            else
                buf = ":" + _nickname + " KICK " + chan + " " + client + " :"+_nickname+"\n";
            _irc->findChanByName(chan)->setMsgToAllClients(buf, _irc);
            _irc->findChanByName(chan)->removeClient(client);
            return;
        }
    }
}
void       Client::methodPing(std::string line){
    std::string msg;
    msg.append(":" + _irc->getServerName() + " PONG " + ":" + _irc->getServerName() + " " + line + "\n");
    allocateResponse(msg);
}
void       Client::methodJoin(std::string line) {
    if (!_isAuthorisedNickUser) {
        _response.addReply(ERR_NOTREGISTERED);
    } else if (line.empty()) {
        _response.addReply(ERR_NEEDMOREPARAMS);
    } else if (_irc->findChanByName(line) && _irc->findChanByName(line)->checkChanClientBanned(_nickname)){
        _response.addReply(ERR_BANNEDFROMCHAN,line);
    } else if (_irc->findChanByName(line) && _irc->findChanByName(line)->isInviteOnly() && !_irc->findChanByName(line)->isInvitedClient(_nickname)) {
        _response.addReply(ERR_INVITEONLYCHAN, line);
    } else {
        if (line[0] == ':')
            line.erase(line.begin());
        if (line[0] != '#' && line[0] != '&')
            line.insert(line.begin(), '#');
        if(_irc->addChannel(line, _nickname)) {
            std::string msg = ":" + _nickname+"!"+_username+"@"+_irc->getPortServer().getIp() + " JOIN " + line + "\n";
            _irc->findChanByName(line)->setMsgToAllClients(msg, _irc);
            methodTopic(line);
            methodNames(line);
        } else {
//                    std::cout << "join: joined already\n";
        }
    }
}
void       Client::methodInvite(std::string line) {
    size_t pos;
    std::string client;
    std::string chan;
    std::string buf;
    if (!_isAuthorisedNickUser) {
        _response.addReply(ERR_NOTREGISTERED);
    } else {
        pos = line.find(' ');
        if (pos!=std::string::npos) {
            client = line.substr(0,pos);
            if(line.size() - pos > 1)
                chan = line.substr(pos+1, line.size()- pos - 1);
        }
        if(chan.empty() || client.empty()) {
            _response.addReply(ERR_NEEDMOREPARAMS);
        } else if(!_irc->findChanByName(chan)) {
            _response.addReply(ERR_NOSUCHCHANNEL,chan);
        } else if (_irc->findChanByName(chan)->checkChanClientBanned(_nickname)){
            _response.addReply(ERR_BANNEDFROMCHAN,chan);
        } else if(!_irc->findChanByName(chan)->findOperator(_nickname)) {
            _response.addReply(ERR_CHANOPRIVSNEEDED,chan);
        } else if(!_irc->findClientByNickName(client)) {
            _response.addReply(ERR_NOSUCHNICK,"",client);
        } else if (_irc->findChanByName(chan) && _irc->findChanByName(chan)->checkChanClientBanned(client)){
            _response.addReply(ERR_CANNOTSENDTOCHAN,chan);
        } else if(_irc->findChanByName(chan)->findClient(client)) {
            _response.addReply(ERR_USERONCHANNEL,chan,client);
        } else {
            _irc->findChanByName(chan)->addClientInvite(client);
            buf = ":" + _nickname + " INVITE :" + client + " :" + chan +"\n";
            _irc->findClientByNickName(client)->allocateResponse(buf);
            _response.addReply(RPL_INVITING,chan,client);
//                    buf = ":"+_irc->getServerName()+" NOTICE @" +chan+" :"+_nickname+" invited "+client+" into channel "+chan+"\n";
//                    allocateResponse(buf);
        }
    }
}
void       Client::methodWho(std::string line) {}