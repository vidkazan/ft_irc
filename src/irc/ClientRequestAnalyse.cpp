//
// Created by Dmitrii Grigorev on 27.03.22.
//
#include "../main.hpp"

        Client::Client(int fd, Irc* irc): _socketFD(fd),\
                                        _status(READING),\
                                        _isAuthorisedPass(0),\
                                        _isAuthorisedNickUser(0),\
                                        _msgUnAuthorisedMsg(1),\
                                        _msgWrongPassMsg(0),\
                                        _msgStarMsg(1),\
                                        _irc(irc) {
                                        }

void        Client::analyseRequest(std::string line)
{
    size_t pos = line.find(' ');
    if(pos != std::string::npos) {
        line.erase(0,pos + 1);
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
            break;
        }
        case PASS:
        {
            if(_isAuthorisedPass)
                break;
            std::cout << "PASS " << _socketFD << " " << line <<  "  \n";
            if(checkPassword(line)) {
                _isAuthorisedPass = 1;
                _msgWrongPassMsg = 0;
            }
            else {
                _msgWrongPassMsg = 1;
                generateResponse();
            }
            break;
        }
        case NICK:
        {
            if(!_isAuthorisedPass)
                break;
            if(_irc->findClientByNickName(line))
                break;
            std::cout << "nickname is SET: " << line << " \n";
            _nickname = line;
//            if(!_nickname.empty() && !_realname.empty() && !_username.empty())
            if(!_nickname.empty()) {
                _isAuthorisedNickUser = true;
                generateResponse();
            }
            break;
        }
        case USER:
        {
            if(!_isAuthorisedPass)
                break;
            if(!_nickname.empty() && !_realname.empty() && !_username.empty()) {
                _isAuthorisedNickUser = true;
                generateResponse();
            }
            break;
        }
        case PRIVMSG:
        {
            if(!_isAuthorisedPass || !_isAuthorisedNickUser)
                break;
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
                if(receiverName[0] == '#') {
                    _irc->printAllChannels();
                    if (_irc->findChanByName(receiverName) != nullptr) {
                        _irc->findChanByName(receiverName)->setMsgToAllClients(msg, _irc);
                    }
                }
                else {
                    _irc->printAllClients();
                    if (_irc->findClientByNickName(receiverName) != nullptr) {
                        _irc->findClientByNickName(receiverName)->allocateResponse(msg);
                        _irc->findClientByNickName(receiverName)->setStatus(WRITING);
                    }
                }
            }
            break;
        }
        case JOIN:
        {
            if(!_isAuthorisedPass || !_isAuthorisedNickUser)
                break;
            _irc->addChannel(line, _nickname);
//            _irc->setMsgToClient(_nickname,msg);
            std::cout << "JOIN " << _socketFD << " " << line <<  "  \n";
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