//
// Created by Dmitrii Grigorev on 27.03.22.
//
#include "../main.hpp"

void        Client::generateResponse()
{
//    std::cout << "log " << _socketFD << " _isAuthorisedPass " << _isAuthorisedPass << "\n" \
//    "_msgUnAuthorisedMsg " << _msgUnAuthorisedMsg << "\n" \
//    "_msgWrongPassMsg " << _msgWrongPassMsg << "\n" \
//    "_msgStarMsg " << _msgStarMsg << "\n";
    std::string bufResp;
    if(_msgUnAuthorisedMsg){
        bufResp += ":ft_irc NOTICE AUTH :please via PASS\n";
        _msgUnAuthorisedMsg = false;
    }
    else {
        switch (_response.getResponseCodes()) {
            case ERR_NORECIPIENT: {
                bufResp += ":ft_irc 411 :No recipient given\n";
                break;
            }
            case ERR_CANNOTSENDTOCHAN: {
                bufResp += ":ft_irc 404 :Cannot send to channel\n";
                break;
            }
            case ERR_NOSUCHNICK: {
                bufResp += ":ft_irc 401 :No such nick\n";
                break;
            }
            case ERR_NOSUCHCHANNEL: {
                bufResp += ":ft_irc 403 :No such channel\n";
                break;
            }
            case ERR_NOTEXTTOSEND: {
                bufResp += ":ft_irc 412 :No text to send\n";
                break;
            }
            case ERR_NEEDMOREPARAMS: {
                bufResp += ":ft_irc 461 :Not enough parameters\n";
                break;
            }
            case ERR_NICKNAMEINUSE: {
                bufResp += ":ft_irc 432 :Nickname is already in use\n";
                break;
            }
            case ERR_NOTREGISTERED: {
                bufResp += ":ft_irc 451 :You have not registered\n";
                break;
            }
            case ERR_NONICKNAMEGIVEN: {
                bufResp += ":ft_irc 431 :No nickname given\n";
                break;
            }
            case ERR_PASSWDMISMATCH: {
                bufResp += ":ft_irc 464 :Password incorrect\n";
                break;
            }
            case ERR_ALREADYREGISTRED: {
                bufResp += ":ft_irc 462 :You may not register\n";
                break;
            }
            case REGISTERED: {
                bufResp += ":ft_irc 001 " + _nickname + " :Welcomeeeeeeee!\n";
                _msgStarMsg = false;
                break;
            }
            case CODE_NOT_SET: {
                return;
            }
        }
    }
    allocateResponse(bufResp);
    _status = WRITING;
    _response.setResponseCode(0);
}
void        Client::allocateResponse(std::string bufResp){
    char *res;
    size_t i=0;
    size_t size = bufResp.size();
    res = (char *)malloc(sizeof (char) * (size));
    for(;i < size;i++){
        res[i] = bufResp[i];
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
        std::cout << "send: " << getSocketFd() << "msg: " << _response.getResponse() << "\n";
        ret = send(_socketFD, _response.getResponse() + _response.getBytesSent(), size, 0x80000); //  SIGPIPE ignore
        if (ret <= 0) {
            std::cout << "send " << _socketFD << ": " << "set to CLOSING" << "  \n";
            Response response;
            setResponse(response);
            Request request;
            _request = request;
            free(_response.getResponse());
            setStatus(CLOSING);
            return;
        }
        _response.addBytesSent(ret);
//        std::cout << "send " << _socketFD << ": " << " sent " << _response.getBytesSent() << " respSize "
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