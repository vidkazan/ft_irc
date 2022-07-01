//
// Created by Dmitrii Grigorev on 27.03.22.
//
#include "Client.hpp"
#include "../main.hpp"

        Client::Client(int fd, Irc* irc,std::string ip): _socketFD(fd),\
                                        _status(READING),\
                                        _hostIp(ip),\
                                        _isAuthorisedPass(0),\
                                        _isAuthorisedNickUser(0),\
                                        _irc(irc), \
                                        _lastCode(CODE_NOT_SET){
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
            methodQuit(line);
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
        case NOTICE: {
            methodPrivmsg(line,1);
            break;
        }
        case PRIVMSG: {
            methodPrivmsg(line,0);
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
        case PART: {
            methodPart(line);
            break;
        }
        case MODE: {
            methodMode(line);
            break;
        }
        case WHO: {
            methodWho(line);
            break;
        }
    }
    generateResponse();
}

bool       Client::checkPassword(std::string p)
{
    return(_irc->checkPassword(p));
}
