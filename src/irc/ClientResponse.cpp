//
// Created by Dmitrii Grigorev on 27.03.22.
//
#include "../main.hpp"

void        Client::generateResponse()
{
    std::string bufResp;
    if(_isAuthorised)
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
                bufResp = "HTTP/1.1 200 OK\r\n";
                _response.setResponseCode(200);
                break;
            }
        }
    }
    else
    {
        bufResp += ":ft_irc Auth please via PASS\n";
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
}
void        Client::sendResponse()
{
    ssize_t ret;
    ssize_t size = 0;
    if(_response.getResponseSize() - _response.getBytesSent() < 1000000000)
        size = _response.getResponseSize() - _response.getBytesSent();
    else
        size = 999999999;
    std::cout << "send: " << getSocketFd() << "msg: " << _response.getResponse() << "\n";
    ret = send(_socketFD, _response.getResponse() + _response.getBytesSent(),size,0x80000); //  SIGPIPE ignore
    if(ret <= 0)
    {
        Response response;
        setResponse(response);
        Request request;
        _request = request;
        free(_response.getResponse());
        setStatus(CLOSING);
        return;
    }
    _response.addBytesSent(ret);
    if(_response.getBytesSent() == (size_t)_response.getResponseSize())
    {
        setStatus(READING);
        free(_response.getResponse());
        Response response;
        setResponse(response);
        Request request;
        _request = request;
    }
}