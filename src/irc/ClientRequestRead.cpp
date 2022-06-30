//
// Created by Dmitrii Grigorev on 27.03.22.
//

#include "Client.hpp"
void        printAsInt(std::string str)
{
            std::cout << "|";
            for(int i=0;i<str.size();i++)
            {
                std::cout << +str[i] << " ";
            }
        std::cout << "|";
}

void        Client::readRequest()
{
    recvBuffer();
    size_t pos;
    while(_request.getBuffer().find('\n')  != std::string::npos)
    {
        pos = _request.getBuffer().find('\n');
        std::string line;
        if (pos != std::string::npos) {
            line = _request.getBuffer().substr(0, pos);
        size_t pos1 = line.find('\r');
        if(pos1 != std::string::npos) {
            line.erase(pos1,1);
        }
        std::cout << "line " << line <<  "\n";
        parseRequest(line);
        analyseRequest(line);
            std::cout << "buf |" << _request.getBuffer() <<  "|\n";
        _request.setBuffer(_request.getBuffer().substr(pos + 1, _request.getBuffer().size() - pos));
            std::cout << "buf erased |" << _request.getBuffer() <<  "|\n";;
        }
    }
}

void        Client::recvBuffer(){
    ssize_t ret;
    char buf[100000];
    bzero(&buf, 100000);
    ret = recv(_socketFD, &buf, 99999, 0);
    if (ret == -1 || ret == 0)
    {
        methodQuit(":Client closed connection");
        free(_response.getResponse());
        _status = CLOSING;
        return;
    }
    _request.addBytesReceieved(ret);
    _request.appendBuffer(buf, ret);
    std::cout << _request.getBuffer() << "\n";
}