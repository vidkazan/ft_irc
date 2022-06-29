#pragma once

enum RequestMethod{
    // replies
	NO_METHOD,
	OTHER_METHOD,
    PASS,
    NICK,
    USER,
    PRIVMSG,
    JOIN,
    QUIT,
    PING,
    LIST,
    NAMES,
    PART,
    TOPIC,
    NOTICE,
    INVITE,
    MODE,
    KICK,
    WHO,
    USERHOST,
};

class Request{
private:
	// header
	short                           _id;
	std::string                     _type;
	std::string                     _buffer;
	size_t                          _count;
	RequestMethod	                _requestMethod;
    size_t                          _bytesReceieved;
public:
	Request():	_id(0), \
				_type(""),\
				_buffer(""),\
				_count(0), \
				_requestMethod(NO_METHOD), \
                _bytesReceieved(0)
	{
		while(_id < 1){
			_id = rand();
		}
	};
	virtual                         ~Request(){}
	RequestMethod                   &getRequestMethod(){return _requestMethod;};
	void	                        setRequestMethod(int m){_requestMethod = static_cast<RequestMethod>(m);}
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
	void                            setCounter(size_t count){_count = count;}
};