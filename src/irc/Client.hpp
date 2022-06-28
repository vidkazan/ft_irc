#pragma once
#include <iostream>
#include "../irc/Request.hpp"
#include "../irc/Response.hpp"
#include "../server/Irc.hpp"

#define READING 0
#define WRITING 1
#define CLOSING 2

class Irc;
class Chan;
class Response;
class Client 
{
private:
	int                              _socketFD;
	int                              _status;
	Response                         _response;
	Request                          _request;
    Irc*                             _irc;
//                      Names
    std::string                      _nickname;
    std::string                      _username;
    std::string                      _realname;
    std::string                      _hostname;
    std::string                      _servername;
//                      Auth
    bool                             _isAuthorisedPass;
    bool                             _isAuthorisedNickUser;
//                      Chans
public:
                Client(int fd, Irc* irc);
	            ~Client(){};
    std::string getNickName(){return _nickname;}
	int         getStatus() const {return _status;};
    Request&    getRequest(){return _request;};
    Response&   getResponse(){return _response;};
	int         getSocketFd() const{return _socketFD;}
	void        setStatus(int status){_status = status;}
    void        setResponse(const Response&response){_response = response;}

//              ClientRequestParse.cpp

    void        parseRequest(std::string);

//              ClientRequestAnalyse.cpp

    void        analyseRequest(std::string);
    bool        checkPassword(std::string);
    //          methods
    void methodNames(std::string);
    void methodNick(std::string);
    void methodPass(std::string);
    void methodTopic(std::string);
    void methodUser(std::string);
    void methodPrivmsg(std::string);
    void methodMode(std::string);
    void methodPart(std::string);
    void methodKick(std::string);
    void methodPing(std::string);
    void methodJoin(std::string);
    void methodInvite(std::string);
    void methodWho(std::string);

//              ClientResponse.cpp

    void        generateResponse();
    void        allocateResponse(std::string bufResp);
    void        sendResponse();

//              ClientReadRequest.cpp

    void        readRequest();
    void        recvBuffer();
    std::string stringToUpper(std::string str);

};

