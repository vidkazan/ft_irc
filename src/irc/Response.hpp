#pragma once
#include "../main.hpp"

enum responseCodeStates{
//    "<nickname> :No such nick/channel"
//    - Used to indicate the nickname parameter supplied to a
//    command is currently unused.
    ERR_NOSUCHNICK = 401,

//    "<channel name> :No such channel"
//    - Used to indicate the given channel name is invalid.
    ERR_NOSUCHCHANNEL = 403,

//    "<channel name> :Cannot send to channel"
//    - Sent to a user who is either (a) not on a channel
//    which is mode +n or (b) not a chanop (or mode +v) on
//    a channel which has mode +m set and is trying to send
//    a PRIVMSG message to that channel.
    ERR_CANNOTSENDTOCHAN = 404,

//    "<channel name> :You have joined too many \
//                         channels"
//    - Sent to a user when they have joined the maximum
//    number of allowed channels and they try to join
//    another channel.
//    ERR_TOOMANYCHANNELS = 405,

//    "<nickname> :There was no such nickname"
//    - Returned by WHOWAS to indicate there is no history
//    information for that nickname.
//    ERR_WASNOSUCHNICK = 406,

//    "<target> :Duplicate recipients. No message \
//        delivered"
//    - Returned to a client which is attempting to send a
//    PRIVMSG/NOTICE using the user@host destination format
//    and for a user@host which has several occurrences.
//    has several occurrences.
//    ERR_TOOMANYTARGETS = 407,

//    ":No recipient given (<command>)"
    ERR_NORECIPIENT = 411,

//    ":No text to send"
//    - 412 - 414 are returned by PRIVMSG to indicate that
//    the message wasn't delivered for some reason.
    ERR_NOTEXTTOSEND  = 412,

//    "<command> :Unknown command"
//    - Returned to a registered client to indicate that the
//    command sent is unknown by the server.
    ERR_UNKNOWNCOMMAND = 421,

//    ":File error doing <file op> on <file>"
//    - Generic error message used to report a failed file
//    operation during the processing of a message.
    ERR_FILEERROR = 424,

//    ":No nickname given"
//    - Returned when a nickname parameter expected for a
//    command and isn't found.
    ERR_NONICKNAMEGIVEN = 431,

//    "<nick> :Erroneus nickname"
//    - Returned after receiving a NICK message which contains
//    characters which do not fall in the defined set.  See
//    section x.x.x for details on valid nicknames.
//    ERR_ERRONEUSNICKNAME = 432,

//    "<nick> :Nickname is already in use"
//    - Returned when a NICK message is processed that results
//    in an attempt to change to a currently existing
//    nickname.
    ERR_NICKNAMEINUSE = 433,

//    "<nick> :Nickname collision KILL"
//    - Returned by a server to a client when it detects a
//    nickname collision (registered of a NICK that
//    already exists by another server).
//    ERR_NICKCOLLISION = 436,

//    "<nick> <channel> :They aren't on that channel"
//    - Returned by the server to indicate that the target
//    user of the command is not on the given channel.
    ERR_USERNOTINCHANNEL = 441,

//    "<channel> :You're not on that channel"
//    - Returned by the server whenever a client tries to
//    perform a channel effecting command for which the
//    client isn't a member.
    ERR_NOTONCHANNEL = 442,

//    ":You have not registered"
//    - Returned by the server to indicate that the client
//    must be registered before the server will allow it
//    to be parsed in detail.
    ERR_NOTREGISTERED = 451,

//    "<command> :Not enough parameters"
//    - Returned by the server by numerous commands to
//    indicate to the client that it didn't supply enough
//    parameters.
    ERR_NEEDMOREPARAMS = 461,

//    ":You may not reregister"
//    - Returned by the server to any link which tries to
//    change part of the registered details (such as
//    password or user details from second USER message).
    ERR_ALREADYREGISTRED = 462,

//    ":Password incorrect"
//    - Returned to indicate a failed attempt at registering
//    a connection for which a password was required and
//    was either not given or incorrect.
    ERR_PASSWDMISMATCH = 464,

//    ":You are banned from this server"
//    - Returned after an attempt to connect and register
//    yourself with a server which has been setup to
//    explicitly deny connections to you.
    ERR_YOUREBANNEDCREEP = 465,

//    ":Permission Denied- You're not an IRC operator"
//    - Any command requiring operator privileges to operate
//    must return this error to indicate the attempt was
//    unsuccessful.
//    ERR_NOPRIVILEGES = 481,

//    "<channel> :You're not channel operator"
//    - Any command requiring 'chanop' privileges (such as
//    MODE messages) must return this error if the client
//    making the attempt is not a chanop on the specified
//    channel.
    ERR_CHANOPRIVSNEEDED = 482,

//    "<channel> :Cannot join channel (+i)"
    ERR_INVITEONLYCHAN = 473,

//    "<channel> :Channel key already set
//    ERR_KEYSET = 467,

//    "<char> :is unknown mode char to me"
    ERR_UNKNOWNMODE = 472,

    // PASS
//    ERR_NEEDMOREPARAMS , +
//    ERR_ALREADYREGISTRED, +
//    ERR_PASSWDMISMATCH, +

    // NICK
//    ERR_NONICKNAMEGIVEN, +
//    ERR_NICKNAMEINUSE, +
//    ERR_ERRONEUSNICKNAME, +
//    ERR_NICKCOLLISION, +

    // JOIN
//    ERR_NEEDMOREPARAMS, +
//    ERR_INVITEONLYCHAN , +
//    ERR_CHANNELISFULL,
//    ERR_NOSUCHCHANNEL,+
//    ERR_BANNEDFROMCHAN,
//    ERR_BADCHANNELKEY,
//    ERR_TOOMANYCHANNELS,

    // PART
//    ERR_NOTONCHANNEL,+
//    ERR_NOSUCHCHANNEL,+

    // MODE
//    ERR_NEEDMOREPARAMS +
//    ERR_NOSUCHCHANNEL +
//    ERR_CHANOPRIVSNEEDED +
//    ERR_USERNOTONCHANNEL +
//    ERR_UNKNOWNMODE +

    // TOPIC
//    ERR_NEEDMOREPARAMS, +
//    RPL_NOTOPIC,
//    ERR_CHANOPRIVSNEEDED,+
//    ERR_NOTONCHANNEL, +

//     LIST
//    ERR_NOSUCHSERVER+

    // PRIVMSG
//    ERR_NORECIPIENT, +
//    ERR_CANNOTSENDTOCHAN, +
//    ERR_NOSUCHNICK, +
//    ERR_NOTEXTTOSEND, +
//    ERR_TOOMANYTARGETS,

    //KICK
//    ERR_NEEDMOREPARAMS +
//    ERR_NOTONCHANNEL +
//    ERR_NOSUCHCHANNEL +
//    ERR_CHANOPRIVSNEEDED+

    // INVITE
//    ERR_NEEDMOREPARAMS +
//    ERR_NOSUCHANNEL +
//    ERR_CHANOPRIVSNEEDED +
//    ERR_NOSUCHNICK +

//    "<user> <channel> :is already on channel"
//    - Returned when a client tries to invite a user to a
//    channel they are already on.
    ERR_USERONCHANNEL = 443,

    REGISTERED = 1,
    CODE_NOT_SET = 0,
    ERR_HEISBANNEDCREEP = 666,
};

class Response{
private:
	size_t              _bytesSent;
	char*               _response;
	ssize_t             _responseSize;
	responseCodeStates  _responseCodes;

public:
	Response() : _bytesSent(0),\
				_response(nullptr), \
				_responseSize(-1), \
                _responseCodes(CODE_NOT_SET){};

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
	void                setBytesSent(size_t bytes){_bytesSent = bytes;}
	void                addBytesSent(size_t addBytes){_bytesSent += addBytes;}

};