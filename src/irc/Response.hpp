#pragma once
#include "../main.hpp"

enum responseCodeStates{
	OK = 200,
	BAD_REQUEST = 400,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	PAYLOAD_TOO_LARGE = 413,
    CODE_NOT_SET = 0
};

enum responseType{
    PRIVATE,
    CHANNEL
};

class Response{
private:
	size_t              _bytesSent;
	char*               _response;
	ssize_t             _responseSize;
	bool                _requestIsValid;
	bool                _methodIsAllowed;
	responseCodeStates  _responseCodes;
    std::string         _msgBuf;
    bool                _toCloseTheConnection;

public:
	Response() : _bytesSent(0),\
				_response(nullptr), \
				_responseSize(-1), \
				_requestIsValid(true), \
				_methodIsAllowed(false), \
                _responseCodes(CODE_NOT_SET){};
    bool                toCloseTheConnection(){return _toCloseTheConnection;}
    void                setToCloseTheConnection(bool to){_toCloseTheConnection = to;}
    void                setMsgBuf(std::string msg){
        _msgBuf = msg;
    }
    std::string         getMsgBuf(){return _msgBuf;}
	virtual             ~Response(){};
	responseCodeStates  &getResponseCodes(){return _responseCodes;};
	void                setResponseCode(int code){_responseCodes = static_cast<responseCodeStates>(code);}
	size_t              getBytesSent() const{return _bytesSent;}
	ssize_t             getResponseSize() const{return _responseSize;}
	char*               getResponse() const {return _response;};
	void                setResponse(char *resp, size_t size){
		_responseSize = size;
		_response = resp;
	};
	void                setMethodIsAllowed(bool methodIsAllowed){_methodIsAllowed = methodIsAllowed;}
	bool                isRequestIsValid() const{return _requestIsValid;}
	void                setRequestIsValid(bool requestIsValid){_requestIsValid = requestIsValid;}
	void                setBytesSent(size_t bytes){_bytesSent = bytes;}
	void                addBytesSent(size_t addBytes){_bytesSent += addBytes;}

};