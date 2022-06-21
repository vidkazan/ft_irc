#pragma once
#include "../main.hpp"

enum RequestMethod{
	NO_METHOD,
	OTHER_METHOD,
    PASS,
    NICK,
    USER
};

enum ReadStatus{
	REQUEST_READ_WAITING_FOR_HEADER,
 	REQUEST_READ_HEADER,
 	REQUEST_READ_COMPLETE,
};

enum RequestErrors {
	NO_ERROR,
	ERROR_REQUEST_NOT_VALID,
	ERROR_METHOD_NOT_ALLOWED,
	ERROR_PATH_NOT_AVAILABLE,
	ERROR_BODY_OVER_MAX_SIZE,
};

class Request{
private:
	// header
	short                           _id;
	std::string                     _type;
	std::string                     _buffer;
	ssize_t                         _maxBodySize;
	size_t                          _count;
	ReadStatus		                _readStatus;
	RequestMethod	                _requestMethod;
	RequestErrors	                _requestErrors;
    size_t                          _bytesReceieved;
public:
	Request():	_id(0), \
				_type(""),\
				_buffer(""),\
				_maxBodySize(-1), \
				_count(0), \
				_readStatus(REQUEST_READ_WAITING_FOR_HEADER), \
				_requestMethod(NO_METHOD), \
				_requestErrors(NO_ERROR), \
                _bytesReceieved(0)
	{
		while(_id < 1){
			_id = rand();
		}
	};
	virtual                         ~Request(){}
	ReadStatus                      getReadStatus() const{return _readStatus;}
	RequestMethod                   &getRequestMethod(){return _requestMethod;};
	RequestErrors                   &getRequestErrors(){return _requestErrors;};
	void	                        setReadStatus(int readStatus){_readStatus = static_cast<ReadStatus>(readStatus);}
	void	                        setRequestMethod(int m){_requestMethod = static_cast<RequestMethod>(m);}
	void	                        setRequestErrors(int m){_requestErrors = static_cast<RequestErrors>(m);}
	short                           getRequestId(){return _id;};
	const std::string               &getBuffer() const {return _buffer;};
	std::string                     getType() const {return  _type;};
	void                            setBuffer(const std::string & req){
		this->cleanBuffer();
		_buffer = req;
	};
	void                            appendBuffer(char *str, size_t size){
		_buffer.append(str, size);
	}
	void                            setType(const std::string & type){_type = type;};
	void                            cleanBuffer(){_buffer.erase();}
	size_t                          getCounter() const{return _count;}
    size_t                          getBytesReceieved() const{return _bytesReceieved;}
    void                          addBytesReceieved(size_t n)
    {
        if(n>0)
        _bytesReceieved += n;
    }
	ssize_t                         getMaxBodySize() const{return _maxBodySize;}
	void                            setMaxBodySize(ssize_t size){_maxBodySize = size;}
	void                            setCounter(size_t count){_count = count;}
};