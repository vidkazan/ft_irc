//
// Created by Dmitrii Grigorev on 27.03.22.
//
#include "../main.hpp"

void        Client::parseRequestHeader()
{
        parseRequestTypeOptionVersion(_request.getBuffer());
        analyseRequest();
}

void        Client::parseRequestTypeOptionVersion(std::string str)
{
    size_t pos = str.find(' ');
    if(pos != std::string::npos) {
        _request.setType(str.substr(0, pos));
        str.erase(0,pos+1);
        if(_request.getType() == "PASS")
            _request.setRequestMethod(PASS);
        else if(_request.getType() == "NICK")
            _request.setRequestMethod(NICK);
        else if(_request.getType() == "USER")
            _request.setRequestMethod(NICK);
        else
            _request.setRequestMethod(NO_METHOD);
    }
    else
        _request.setRequestMethod(NO_METHOD);
    _request.setReadStatus(REQUEST_READ_COMPLETE);
}
