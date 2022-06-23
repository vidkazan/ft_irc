#pragma once
#include "../main.hpp"

class Irc;
class Chan;

class Client {
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
//                      Auth
    bool                             _isAuthorisedPass;
    bool                             _isAuthorisedNickUser;
    bool                             _msgUnAuthorisedMsg;
    bool                             _msgWrongPassMsg;
    bool                             _msgStarMsg;
//                      Chans
    std::vector<Chan>                _chans;

public:
                Client(int fd, Irc* irc);
	            ~Client(){};
    std::string getNickName(){return _nickname;}
    bool        isAuth(){return _isAuthorisedPass;}
    void        setIsAuthorised(){_isAuthorisedPass = true;}
	int         getStatus() const {return _status;};
    Request&    getRequest(){return _request;};
    Response&   getResponse(){return _response;};
	int         getSocketFd() const{return _socketFD;}
	void        setStatus(int status){_status = status;}
    void        setResponse(const Response&response){_response = response;}
    void        printStates(const std::string& place)
    {
        std::cout << YELLOW << place << "\n";
        switch (_request.getRequestMethod()) {
            case 0:
                std::cout << "NO_METHOD ";
                break;
            case 1:
                std::cout << "OTH ";
                break;
            case 2:
                std::cout << "PASS ";
                break;
            case 3:
                std::cout << "NICK ";
                break;
            case 4:
                std::cout << "USER ";
                break;
            case 5:
                std::cout << "PRIVMSG ";
                break;
            case 6:
                std::cout << "JOIN ";
                break;
            case 7:
                std::cout << "QUIT ";
                break;
            case 8:
                std::cout << "PONG ";
                break;
        }
        std::cout << "\n";
        switch (_request.getRequestErrors()) {
            case 0:
                std::cout << "NO_ERROR ";
                break;
            case 1:
                std::cout << "ERROR_REQUEST_NOT_VALID ";
                break;
            case 2:
                std::cout << "ERROR_METHOD_NOT_ALLOWED ";
                break;
            case 3:
                std::cout << "ERROR_PATH_NOT_AVAILABLE ";
                break;
            case 4:
                std::cout << "ERROR_BODY_OVER_MAX_SIZE ";
                break;
        }
        std::cout << WHITE << "\n";
    }

//              ClientRequestParse.cpp

    void        parseRequest(std::string);

//              ClientRequestAnalyse.cpp

    void        analyseRequest(std::string);
    bool        checkPassword(std::string);

//              ClientResponse.cpp

    void        generateResponse();
    void        allocateResponse(std::string bufResp);
    void        sendResponse();

//              ClientReadRequest.cpp

    void        readRequest();
    void        recvBuffer();

//              ClientRequestChans.cpp
    void        addChannel(Chan&);
    void        removeChannel(Chan&);

};

