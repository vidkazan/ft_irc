//
// Created by Dmitrii Grigorev on 27.03.22.
//

#include "../main.hpp"

void        Client::parseRequest(std::string str)
{
    size_t pos = str.find(' ');
    if(pos != std::string::npos) {
        _request.setType(str.substr(0, pos));
        str.erase(0,pos+1);
    }
    else if(!str.empty()) {
        _request.setType(str);
    }
    else {
        _request.setRequestMethod(NO_METHOD);
        return;
    }
    if(_request.getType() == "PASS")
        _request.setRequestMethod(PASS);
    else if(_request.getType() == "NICK")
        _request.setRequestMethod(NICK);
    else if(_request.getType() == "USER")
        _request.setRequestMethod(USER);
    else if(_request.getType() == "JOIN")
        _request.setRequestMethod(JOIN);
    else if(_request.getType() == "PING")
        _request.setRequestMethod(PING);
    else if(_request.getType() == "PRIVMSG")
        _request.setRequestMethod(PRIVMSG);
    else if(_request.getType() == "QUIT")
    {
        std::cout << "QUIT\n";
        _request.setRequestMethod(QUIT);
    }
    else {
        _request.setRequestMethod(OTHER_METHOD);
    }
}
