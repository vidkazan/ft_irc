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
	size_t              getBytesSent() const{return _bytesSent;}
	ssize_t             getResponseSize() const{return _responseSize;}
	char*               getResponse() const {return _response;};
	void                setResponse(char *resp, size_t size){
		_responseSize = size;
		_response = resp;
	};
	void                addBytesSent(size_t addBytes){_bytesSent += addBytes;}
    void addReply(replyCodes code, std::string chan="",std::string receiver="",std::string senderNick="",std::string senderUser="",std::string senderHost="",std::string msg="") {
        _replyQueue.push_back(Reply(code,chan,receiver,senderNick,senderUser,senderHost,msg));
    }

    void addMsg(replyCodes code, std::string chan="",std::string receiver="",std::string senderNick="",std::string senderUser="",std::string senderHost="",std::string msg="") {
        _replyQueue.push_back(Reply(code,chan,receiver,senderNick,senderUser,senderHost,msg));
    }
    void addMsg(Reply msg) {
        _replyQueue.push_back(msg);
    }

    Reply* getLastReply() {
        if(_replyQueue.empty())
            return nullptr;
        else
            return &_replyQueue.back();
    }
    std::deque<Reply>::iterator getFrontReply() {
            return _replyQueue.begin();
    }
    std::deque<Reply>::iterator getEnd() {
        return _replyQueue.end();
    }
    void popFrontReply(){
        _replyQueue.pop_front();
    }

};