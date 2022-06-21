#pragma once
#include "../main.hpp"


class Client {
private:
	int                              _socketFD;
	int                              _status;
	Response                         _response;
	Request                          _request;
    bool                             _isAuthorised;

public:
	            Client(int fd): _socketFD(fd), _status(WRITING), _isAuthorised(0) {};
	            ~Client(){};
    bool        isAuth(){return _isAuthorised;}
    void        setIsAuthorised(){_isAuthorised = true;}
	int         getStatus() const {return _status;};
    Request&    getRequest(){return _request;};
    Response&    getResponse(){return _response;};
	int         getSocketFd() const{return _socketFD;}
	void        setStatus(int status){_status = status;}
    void        setResponse(const Response&response){_response = response;}
    void        printStates(const std::string& place)
    {
        std::cout << YELLOW << place << "\n";
        switch (_request.getReadStatus()) {
            case 0:
                std::cout << "REQUEST_READ_WAITING_FOR_HEADER ";
                break;
            case 1:
                std::cout << "REQUEST_READ_HEADER ";
                break;
            case 2:
                std::cout << "REQUEST_READ_COMPLETE ";
                break;
        }
        std::cout << "\n";
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

    void        parseRequestHeader();
    void        parseRequestTypeOptionVersion(std::string str);

//              ClientRequestAnalyse.cpp

    void        analyseRequest();

//              ClientResponse.cpp

    void        generateResponse();
    void        allocateResponse(std::string bufResp);
    void        sendResponse();

//              ClientReadRequest.cpp

    void        readRequest();
    void        recvBuffer();
};

