//
// Created by Dmitrii Grigorev on 27.03.22.
//

#include "../main.hpp"

void        Client::readRequest()
{
    recvBuffer();
    if (_request.getBuffer().find("\r\n\r\n") != std::string::npos) {
        parseRequestHeader();
        _status = WRITING;
    }
}
void        Client::recvBuffer(){
    ssize_t ret;
    char buf[100000];
    bzero(&buf, 100000);
    ret = recv(_socketFD, &buf, 99999, 0);
    if (ret == -1 || ret == 0)
    {
        free(_response.getResponse());
        _status = CLOSING;
        return;
    }
    _request.addBytesReceieved(ret);
    _request.appendBuffer(buf, ret);
}