//
// Created by Dmitrii Grigorev on 27.03.22.
//
#include "../main.hpp"

        Client::Client(int fd, Irc* irc): _socketFD(fd),\
                                        _status(READING),\
                                        _isAuthorisedPass(0),\
                                        _isAuthorisedNickUser(0),\
                                        _msgUnAuthorisedMsg(1),\
                                        _msgStarMsg(1),\
                                        _irc(irc) {
                                        }

void        Client::analyseRequest(std::string line)
{
    size_t pos = line.find(' ');
    if(pos != std::string::npos) {
        line.erase(0,pos + 1);
    }
    else if (_request.getRequestMethod() != NO_METHOD && _request.getRequestMethod() != OTHER_METHOD) {
        line = "";
    }
    std::cout << "line erased:|" << line << "|\n";
    switch (_request.getRequestMethod()) {
        case TOPIC: {
            break;
        }
        case NO_METHOD: {
            _response.setResponseCode(CODE_NOT_SET);
            break;
        }
        case OTHER_METHOD: {
            if (_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_UNKNOWNCOMMAND);
                generateResponse();
            }
            break;
        }
        case QUIT: {
            _status = CLOSING;
            break;
        }
        case PASS: {
            if (line[0] == ':')
                line.erase(line.begin());
            if (_isAuthorisedPass) {
                std::cout << _socketFD << " PASS ERR_ALREADYREGISTRED \n";
                _response.setResponseCode(ERR_ALREADYREGISTRED);
            } else if (line.empty()) {
                std::cout << _socketFD << " PASS ERR_NEEDMOREPARAMS \n";
                _response.setResponseCode(ERR_NEEDMOREPARAMS);
            } else if (checkPassword(line)) {
                _isAuthorisedPass = 1;
                std::cout << _socketFD << " PASS CORRECT \n";
                break;
            } else {
                std::cout << _socketFD << " PASS ERR_PASSWDMISMATCH \n";
                _response.setResponseCode(ERR_PASSWDMISMATCH);
            }
            generateResponse();
            break;
        }
        case NICK: {
            if (_isAuthorisedNickUser) {
                std::cout << _socketFD << "NICK ERR_ALREADYREGISTRED \n";
                _response.setResponseCode(ERR_ALREADYREGISTRED);
            } else if (!_isAuthorisedPass) {
                std::cout << _socketFD << "NICK ERR_NOTREGISTERED \n";
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else if (line.empty()) {
                std::cout << _socketFD << "NICK ERR_NONICKNAMEGIVEN \n";
                _response.setResponseCode(ERR_NONICKNAMEGIVEN);
            } else if (_irc->findClientByNickName(line)) {
                std::cout << _socketFD << "NICK ERR_NICKNAMEINUSE \n";
                _response.setResponseCode(ERR_NICKNAMEINUSE);
                _attrs.push_back(line);
            } else {
                std::cout << "NICKNAME is SET: " << line << " \n";
                _nickname = line;
                _isAuthorisedNickUser = true;
                _response.setResponseCode(REGISTERED);
            }
            generateResponse();
            break;
        }
        case USER: {
            break;
        }
        case NOTICE:
        case PRIVMSG: {
            if (!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else if (line.empty()) {
                _response.setResponseCode(ERR_NORECIPIENT);
            } else {
//                std::cout << "PRIVMSG: buffer |" << line << "|\n";
                std::string msg;
                std::string receiverName;
                size_t pos = line.find(':');
                if (pos != std::string::npos) {
                    receiverName = line.substr(0, pos - 1);
//                    std::cout << "analyse: PRIVMSG: receiverName " << receiverName << "\n";
                    msg = ":" + _nickname + " PRIVMSG " + receiverName + " " +
                          line.substr(pos, _request.getBuffer().size() - pos);
//                    std::cout << "analyse: PRIVMSG: msg " << msg << "\n";
                    msg.append("\n");
                    if (receiverName[0] == '#' || receiverName[0] == '&') {
                        if (_irc->findChanByName(receiverName) == nullptr) {
                            _response.setResponseCode(ERR_NOSUCHCHANNEL);
                            _attrs.push_back(receiverName);
                        } else if (!_irc->findChanByName(receiverName)->findClient(_nickname)) {
                            _response.setResponseCode(ERR_CANNOTSENDTOCHAN);
                            _attrs.push_back(receiverName);
                        } else {
                            _irc->findChanByName(receiverName)->setMsgToAllClients(_nickname,msg, _irc);
                            return;
                        }
                    } else {
                        if (_irc->findClientByNickName(receiverName) != nullptr) {
                            _irc->findClientByNickName(receiverName)->allocateResponse(msg);
                            _irc->findClientByNickName(receiverName)->setStatus(WRITING);
                            return;
                        } else {
                            _response.setResponseCode(ERR_NOSUCHNICK);
                            _attrs.push_back(receiverName);
                        }
                    }
                } else {
                    _response.setResponseCode(ERR_NOTEXTTOSEND);
                }
            }
            generateResponse();
            break;
        }
        case INVITE: {
            if (!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else if (line.empty()) {
                _response.setResponseCode(ERR_NEEDMOREPARAMS);
            }
            break;
        }
        case JOIN: {
            if (!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else if (line.empty()) {
                _response.setResponseCode(ERR_NEEDMOREPARAMS);
            } else {
                if (line[0] == ':')
                    line.erase(line.begin());
                if (line[0] != '#' && line[0] != '&')
                    line.insert(line.begin(), '#');
                if(_irc->addChannel(line, _nickname)) {
                    std::string msg = ":" + _nickname + " JOIN :" + line + "\n";
                    generateNamesRPL(line);
                    _irc->findChanByName(line)->setMsgToAllClients(msg, _irc);
                } else {
//                    std::cout << "join: joined already\n";
                }
                return;
            }
            generateResponse();
            break;
        }
        case PING: {
            std::string msg;
            msg.append(":" + _irc->getServerName() + " PONG " + ":" + _irc->getServerName() + ":" + line + "\n");
            allocateResponse(msg);
            break;
        }
        case NAMES: {
            if (!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else if (line.empty()) {
                _response.setResponseCode(ERR_NEEDMOREPARAMS);
            } else if (!_irc->findChanByName(line)) {
                _response.setResponseCode(ERR_NOSUCHCHANNEL);
                _attrs.push_back(line);
            } else {
                generateNamesRPL(line);
                break;
            }
            generateResponse();
            break;
        }
        case LIST: {
            if (!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else {
                allocateResponse(_irc->channelsList(_nickname));
                break;
            }
            generateResponse();
            break;
        }
        case KICK: {
            size_t pos, pos1;
            std::string chan;
            std::string client;
            std::string buf;
            std::string msg;
            if (!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
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
                    _response.setResponseCode(ERR_NEEDMOREPARAMS);
                    _attrs.push_back("KICK");
                } else if(!_irc->findChanByName(chan)) {
                    _response.setResponseCode(ERR_NOSUCHCHANNEL);
                    _attrs.push_back(chan);
                } else if(!_irc->findChanByName(chan)->findOperator(_nickname)) {
                    _response.setResponseCode(ERR_CHANOPRIVSNEEDED);
                    _attrs.push_back(chan);
                } else if(!_irc->findChanByName(chan)->findClient(client)) {
                    _response.setResponseCode(ERR_USERNOTINCHANNEL);
                    _attrs.push_back(client);
                    _attrs.push_back(chan);
                } else {
//                    :pepe11!~1@89.232.114.120 KICK #pepe pepe22 :pepe11
                    buf = ":" + _nickname + " KICK " + chan + " " + client + " :"+_nickname + "\n";
                    _irc->findChanByName(chan)->setMsgToAllClients(buf, _irc);
                    _irc->findChanByName(chan)->removeClient(client);
                    return;
                }
            }
            generateResponse();
            break;
        }
        case PART:
        {
            size_t pos = line.find(':');
            if(pos != std::string::npos){
                line.erase(pos-1, line.size() - pos +1);
            }
//            std::cout << "ch:|" << line << "|\n";
            if (!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else if(line.empty()) {
                _response.setResponseCode(ERR_NEEDMOREPARAMS);
            } else if (!_irc->findChanByName(line)){
                _response.setResponseCode(ERR_NOSUCHCHANNEL);
                _attrs.push_back(line);
            } else if (!_irc->findChanByName(line)->findClient(_nickname)) {
                _response.setResponseCode(ERR_NOTONCHANNEL);
                _attrs.push_back(line);
            } else {
                std::string msg = ":" + _nickname + " PART :" + line + "\n";
                _irc->findChanByName(line)->setMsgToAllClients(_nickname,msg, _irc);
                _irc->findChanByName(line)->removeClient(_nickname);
                break;
            }
            generateResponse();
            break;
        }
        case MODE: {
            if (!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else if (line.empty()) {
                _response.setResponseCode(ERR_NEEDMOREPARAMS);
            }
            generateResponse();
            break;
        }
    }
}
bool        Client::checkPassword(std::string p)
{
    return(_irc->checkPassword(p));
}

void        Client::generateNamesRPL(std::string line)
{
    std::string msg = ":" + _irc->getServerName() + " 353 " + _nickname + " = " +
                      _irc->findChanByName(line)->getChanNames();
    msg += ":" + _irc->getServerName() + " 366 " + _nickname + " " + _irc->findChanByName(line)->getName() + \
                " :End of /NAMES list.\n";
    allocateResponse(msg);
}