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
    if(_isAuthorisedPass && _isAuthorisedNickUser && !_msgStarMsg)
    {
        switch (_request.getRequestErrors()) {
            case ERROR_REQUEST_NOT_VALID: {
                bufResp += "HTTP/1.1 400 Bad Request\n";
                _response.setResponseCode(400);
                break;
            }
            case ERROR_METHOD_NOT_ALLOWED: {
                bufResp = "HTTP/1.1 405 Method Not Allowed\n";
                _response.setResponseCode(405);
                break;
            }
            case ERROR_PATH_NOT_AVAILABLE: {
                bufResp = "HTTP/1.1 404 Not found\n";
                _response.setResponseCode(404);
                break;
            }
            case ERROR_BODY_OVER_MAX_SIZE: {
                bufResp = "HTTP/1.1 413 Payload Too Large\n";
                _response.setResponseCode(413);
                break;
            }
            case NO_ERROR: {
                _response.setResponseCode(200);
                break;
            }
        }
    }
    else
    {
        if(_msgWrongPassMsg) {
            bufResp += ":ft_irc Wrong password\n";
            _msgWrongPassMsg = false;
        }
        else if(_msgUnAuthorisedMsg)
        {
            bufResp += ":ft_irc NOTICE AUTH :please via PASS\n";
            _msgUnAuthorisedMsg = false;
        }
        else if(_isAuthorisedNickUser && _isAuthorisedPass && _msgStarMsg)
        {
            bufResp += ":ft_irc 001 ";
            bufResp += _nickname;
            bufResp += " :Welcomeeeeeeee!";
            bufResp +=" \n";
            _msgStarMsg = false;
        }
    }
    allocateResponse(bufResp);
    _status = WRITING;
}
void        Client::allocateResponse(std::string bufResp){
    char *res;
    size_t i=0;
    size_t size = bufResp.size();
    res = (char *)malloc(sizeof (char) * (size));
    for(;i < size;i++){
        res[i] = bufResp[i];
    }
    _response.setResponse(res,i);
    _status = WRITING;
}
void        Client::sendResponse()
{
    ssize_t ret;
    ssize_t size = 0;
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