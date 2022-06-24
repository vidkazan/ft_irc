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
    else {
        line = "";
    }
    switch (_request.getRequestMethod()) {
        case NO_METHOD: {
            _response.setRequestIsValid(false);
            _request.setRequestErrors(ERROR_REQUEST_NOT_VALID);
            break;
        }
        case OTHER_METHOD: {
            break;
        }
        case QUIT: {
            _status = CLOSING;
            //TODO message
            break;
        }
        case PASS:
        {
            std::cout << "PASS " << _socketFD << " |" << line <<  "| \n";
            if(_isAuthorisedPass)
                _response.setResponseCode(ERR_ALREADYREGISTRED);
            else if(line.empty())
                _response.setResponseCode(ERR_NEEDMOREPARAMS);
            else if(checkPassword(line)) {
                _isAuthorisedPass = 1;
                break;
            }
            else {
                _response.setResponseCode(ERR_PASSWDMISMATCH);
            }
            generateResponse();
            break;
        }
        case NICK:
        {
            if(_isAuthorisedNickUser){
                _response.setResponseCode(ERR_ALREADYREGISTRED);
            }
            else if(!_isAuthorisedPass) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            }
            else if(line.empty()) {
                _response.setResponseCode(ERR_NONICKNAMEGIVEN);
            }
            else if(_irc->findClientByNickName(line)) {
                _response.setResponseCode(ERR_NICKNAMEINUSE);
            }
            else {
                std::cout << "nickname is SET: " << line << " \n";
                _nickname = line;
                _isAuthorisedNickUser = true;
                _response.setResponseCode(REGISTERED);
            }
            generateResponse();
            break;
        }
        case USER:
        {
            break;
        }
        case PRIVMSG:
        {
            if(!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else if(line.empty()) {
                _response.setResponseCode(ERR_NORECIPIENT);
            } else {
                std::cout << "analyse: PRIVMSG: buffer |" << line << "|\n";
                std::string msg;
                std::string receiverName;
                size_t pos = line.find(':');
                if (pos != std::string::npos) {
                    receiverName = line.substr(0, pos - 1);
                    std::cout << "analyse: PRIVMSG: receiverName " << receiverName << "\n";
                    msg = line.substr(pos, _request.getBuffer().size() - pos);
                    std::cout << "analyse: PRIVMSG: msg " << msg << "\n";
                    msg.append("\n");
                    if(receiverName[0] == '#' || receiverName[0] == '&') {
//                        _irc->printAllChannels();
                        if (_irc->findChanByName(receiverName) == nullptr) {
                            _response.setResponseCode(ERR_NOSUCHCHANNEL);
                        } else if (!_irc->findChanByName(receiverName)->findClient(_nickname)) {
                            _response.setResponseCode(ERR_CANNOTSENDTOCHAN);
                        } else {
                            _irc->findChanByName(receiverName)->setMsgToAllClients(msg, _irc);
                            return;
                        }
                    } else {
                        _irc->printAllClients();
                        if (_irc->findClientByNickName(receiverName) != nullptr) {
                            _irc->findClientByNickName(receiverName)->allocateResponse(msg);
                            _irc->findClientByNickName(receiverName)->setStatus(WRITING);
                            return;
                        } else {
                            _response.setResponseCode(ERR_NOSUCHNICK);
                        }
                    }
                } else {
                    _response.setResponseCode(ERR_NOTEXTTOSEND);
                }
            }
            generateResponse();
            break;

        }
        case JOIN:
        {
            if(!_isAuthorisedNickUser) {
                _response.setResponseCode(ERR_NOTREGISTERED);
            } else if(line.empty()) {
                _response.setResponseCode(ERR_NEEDMOREPARAMS);
            }
            else {
                if(line[0]!='#' && line[0]!='&')
                    line.insert(line.begin(),'#');
                _irc->addChannel(line, _nickname);
                std::cout << "JOIN " << _socketFD << " " << line << "  \n";
                return;
            }
            generateResponse();
            break;
        }
        case PING:
        {
            std::string msg;
            msg.append(":" + _irc->getServerName() + " PONG " + ":" + _irc->getServerName() + ":" + line + "\n");
            allocateResponse(msg);
            std::cout << "PING " << _socketFD << " " << line <<  "  \n";
            break;
        }
    }
}
bool        Client::checkPassword(std::string p)
{
    return(_irc->checkPassword(p));
}