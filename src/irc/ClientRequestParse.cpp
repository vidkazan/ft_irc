//
// Created by Dmitrii Grigorev on 27.03.22.
//

#include "Client.hpp"

std::string Client::stringToUpper(std::string str){
    std::string res;
    for(int i=0;i<str.size();i++) {
        res+= toupper(str[i]);
    }
    return res;
}

void        Client::parseRequest(std::string str)
{
    size_t pos = str.find(' ');
    if(pos != std::string::npos) {
        _request.setType(stringToUpper(str.substr(0, pos)));
        str.erase(0,pos+1);
    }
    else if(!str.empty()) {
        _request.setType(stringToUpper(str));
    }
    else {
        _request.setRequestMethod(NO_METHOD);
        return;
    }
    if(_request.getType() == "PASS") {
        _request.setRequestMethod(PASS);
    }
    else if(_request.getType() == "NICK") {
        _request.setRequestMethod(NICK);
    }
    else if(_request.getType() == "USER") {
        _request.setRequestMethod(USER);
    }
    else if(_request.getType() == "JOIN") {
        _request.setRequestMethod(JOIN);
    }
    else if(_request.getType() == "PING") {
        _request.setRequestMethod(PING);
    }
    else if(_request.getType() == "PRIVMSG") {
        _request.setRequestMethod(PRIVMSG);
    }
    else if(_request.getType() == "QUIT") {
        _request.setRequestMethod(QUIT);
    }
    else if(_request.getType() == "LIST") {
        _request.setRequestMethod(LIST);
    }
    else if(_request.getType() == "NAMES") {
        _request.setRequestMethod(NAMES);
    }
    else if(_request.getType() == "PART") {
        _request.setRequestMethod(PART);
    }
    else if(_request.getType() == "NOTICE") {
        _request.setRequestMethod(NOTICE);
    }
    else if(_request.getType() == "TOPIC") {
        _request.setRequestMethod(TOPIC);
    }
    else if(_request.getType() == "INVITE") {
        _request.setRequestMethod(INVITE);
    }
    else if(_request.getType() == "KICK") {
        _request.setRequestMethod(KICK);
    }
    else if(_request.getType() == "MODE") {
        _request.setRequestMethod(MODE);
    }
    else if(_request.getType() == "USERHOST") {
        _request.setRequestMethod(USERHOST);
    }
    else {
        _request.setRequestMethod(OTHER_METHOD);
    }
}
