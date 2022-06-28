#pragma once
#include <fstream>
#include "Reply.hpp"

class Response{
private:
	size_t              _bytesSent;
	char*               _response;
	ssize_t             _responseSize;
    std::deque<Reply>   _replyQueue;

public:
	Response() : _bytesSent(0),\
				_response(nullptr), \
				_responseSize(-1) \
                 {};
	virtual             ~Response(){};
//	replyCodes  &getResponseCodes(){return _responseCodes;};
//	void                setResponseCode(int code){_responseCodes = static_cast<replyCodes>(code);}
	size_t              getBytesSent() const{return _bytesSent;}
	ssize_t             getResponseSize() const{return _responseSize;}
	char*               getResponse() const {return _response;};
	void                setResponse(char *resp, size_t size){
		_responseSize = size;
		_response = resp;
	};
	void                addBytesSent(size_t addBytes){_bytesSent += addBytes;}
    void addReply(replyCodes code, std::string chan="",std::string client="",std::string fullreply="",std::string topic="") {
        _replyQueue.push_back(Reply(code,chan,client,fullreply,topic));
    }
    Reply* getLastReply() {
        if(_replyQueue.empty())
            return nullptr;
        else
            return &_replyQueue.back();
    }
    Reply* getFrontReply() {
            if(_replyQueue.empty())
                return nullptr;
            else
                return &_replyQueue.front();
    }
    void popFrontReply(){
        _replyQueue.pop_front();
    }

};