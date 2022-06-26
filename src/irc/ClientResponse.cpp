//
// Created by Dmitrii Grigorev on 27.03.22.
//

//<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
//<prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
//<command>  ::= <letter> { <letter> } | <number> <number> <number>
//<SPACE>
//<params>
//<middle>
//::=''{''}
//::= <SPACE> [ ':' <trailing> | <middle> <params> ]
//::= <Any *non-empty* sequence of octets not including SPACE
//or NUL or CR or LF, the first of which may not be ':'>
//<trailing> ::= <Any, possibly *empty*, sequence of octets not including
//        NUL or CR or LF>
//<crlf>     ::= CR LF

//<target>::= <to> [ "," <target> ]
//<to> ::= <channel> | <user> '@' <servername> | <nick> | <mask>
//<channel> ::= ('#' | '$') <chstring>
//<nick>::= <letter> { <letter> | <number> | <special> }
//<mask>::= <any 8bit code except SPACE, BELL, NUL, CR, LF and
//<chstring>::= ('#' | '&') <chstring>
//              comma (',')>







#include "../main.hpp"

void        Client::generateResponse()
{
//    std::cout << "log " << _socketFD << " _isAuthorisedPass " << _isAuthorisedPass << "\n" \
//    "_msgUnAuthorisedMsg " << _msgUnAuthorisedMsg << "\n" \
//    "_msgWrongPassMsg " << _msgWrongPassMsg << "\n" \
//    "_msgStarMsg " << _msgStarMsg << "\n";
    std::string bufResp;
    if(_msgUnAuthorisedMsg){
        bufResp += ":localhost NOTICE AUTH : PASS is requred\n";
        _msgUnAuthorisedMsg = false;
        return;
    }
    else {
        switch (_response.getResponseCodes()) {
            case ERR_NORECIPIENT: {
                bufResp += ":localhost 411 : No recipient given (" + _request.getType() + ")\n";
                break;
            }
            case ERR_CANNOTSENDTOCHAN: {
                bufResp += ":localhost 404 "+_attrs[0]+" :Cannot send to channel\n";
                break;
            }
            case ERR_NOSUCHNICK: {
                bufResp += ":localhost 401 "+_attrs[0]+" :No such nick\n";
                break;
            }
            case ERR_NOSUCHCHANNEL: {
                bufResp += ":localhost 403 "+_attrs[0]+" :No such channel\n";
                break;
            }
            case ERR_NOTEXTTOSEND: {
                bufResp += ":localhost 412 : No text to send\n";
                break;
            }
            case ERR_NEEDMOREPARAMS: {
                bufResp += ":localhost 461 "+_request.getType()+" :Not enough parameters\n";
                break;
            }
            case ERR_NICKNAMEINUSE: {
                bufResp += ":localhost 432 "+_attrs[0]+" :Nickname is already in use\n";
                break;
            }
            case ERR_NOTREGISTERED: {
                bufResp += ":localhost 451 : You have not registered\n";
                break;
            }
            case ERR_NONICKNAMEGIVEN: {
                bufResp += ":localhost 431 : No nickname given\n";
                break;
            }
            case ERR_PASSWDMISMATCH: {
                bufResp += ":localhost 464 : Password incorrect\n";
                break;
            }
            case ERR_ALREADYREGISTRED: {
                bufResp += ":localhost 462 : You may not register\n";
                break;
            }
            case ERR_NOTONCHANNEL: {
                bufResp += ":localhost 442 "+_attrs[0]+" : You're not on that channel\n";
                break;
            }
            case REGISTERED: {
//                bufResp += ":localhost 375 "+_nickname+" :- ft_irc Message of the Day -\n";
//                bufResp += ":localhost 372 " + _nickname + " :Welcomeeeeeeee!\n";
//                bufResp += ":localhost 376 "+_nickname+" :End of /MOTD command.\n";
                bufResp +=":lion.tx.us.dal.net 001 polop :Welcome to the DALnet IRC Network polop!~111@188.162.39.107\n";
                _msgStarMsg = false;
                break;
            }
            case ERR_UNKNOWNCOMMAND: {
                bufResp += ":localhost 421 :Unknown command\n";
                break;
            }
            case ERR_USERNOTINCHANNEL: {
                bufResp += ":localhost 441 "+_attrs[0]+" "+_attrs[1]+" :They aren't on that channel\n";
                break;
            }
            case ERR_USERONCHANNEL: {
                bufResp += ":localhost 443 "+_attrs[0]+" "+_attrs[1]+" :is already on channel\n";
                break;
            }
            case ERR_INVITEONLYCHAN: {
                bufResp += ":localhost 473 "+_attrs[0]+ " :Cannot join channel (+i)\n";
                break;
            }
            case ERR_CHANOPRIVSNEEDED: {
                bufResp += ":localhost 482 "+_attrs[0]+ " :You're not channel operator\n";
                break;
            }
            case ERR_UNKNOWNMODE: {
                bufResp += ":localhost 472 "+_attrs[0]+ " :is unknown mode to me\n";
                break;
            }
            case ERR_YOUREBANNEDCREEP: {
                bufResp += ":localhost 465 :You are banned from this chan\n";
                break;
            }
            case ERR_HEISBANNEDCREEP: {
                bufResp += ":localhost 465b :He is banned from this chan\n";
                break;
            }
            case ERR_FILEERROR:
            case CODE_NOT_SET:
                return;
        }
    }
    allocateResponse(bufResp);
    _status = WRITING;
    _response.setResponseCode(0);
    _attrs.clear();
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
        std::cout << "send: " << getSocketFd() << "msg: " << tmp << "\n";
        ret = send(_socketFD, _response.getResponse() + _response.getBytesSent(), size, 0x80000); //  SIGPIPE ignore
        if (ret <= 0) {
            std::cout << "send " << _socketFD << ": " << "set to CLOSING" << "  \n";
            free(_response.getResponse());
            Response response;
            setResponse(response);
            Request request;
            _request = request;
            setStatus(CLOSING);
            return;
        }
        _response.addBytesSent(ret);
        std::cout << "send " << _socketFD << ": " << " sent " << _response.getBytesSent() << " respSize "
                  << _response.getResponseSize() << "  \n";
    }
    if(_response.getBytesSent() == (size_t)_response.getResponseSize())
    {
        std::cout << "send " << _socketFD << ": " << "set to READING" <<  "  \n";
        setStatus(READING);
        free(_response.getResponse());
        Response response;
        setResponse(response);
        Request request;
        _request = request;
    }
}