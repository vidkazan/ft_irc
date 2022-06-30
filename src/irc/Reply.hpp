#pragma once
#include <vector>

enum replyCodes{

//    REPLIES

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
//    ERR_YOUREBANNEDCREEP = 465,

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

//    "<user> <channel> :is already on channel"
//    - Returned when a client tries to invite a user to a
//    channel they are already on.
    ERR_USERONCHANNEL = 443,

//     "<channel> :No topic is set"
    RPL_NOTOPIC = 331,

//     "<channel> :<topic>"
    RPL_TOPIC = 332,

//    "Channel :Users  Name"
    RPL_LISTSTART=321,
//    "<channel> <# visible> :<topic>"
    RPL_LIST=322,
//    ":End of /LIST"
    RPL_LISTEND=323,

//    "<channel> <nick>"
//    - Returned by the server to indicate that the
//    attempted INVITE message was successful and is
//    being passed onto the end client.
    RPL_INVITING=341,

//    "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
    RPL_NAMREPLY = 353,

//    "<channel> :End of /NAMES list"
//    - To reply to a NAMES message, a reply pair consisting
//    of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
//    server back to the client.  If there is no channel
//    found as in the query, then only RPL_ENDOFNAMES is
//    returned.  The exception to this is when a NAMES
//    message is sent with no parameters and all visible
//    channels and contents are sent back in a series of
//    RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
//    the end.
    RPL_ENDOFNAMES = 366,

//    ":- <server> Message of the day - "
    RPL_MOTDSTART =375,

//    ":- <text>"
    RPL_MOTD = 372,

//    ":End of /MOTD command"
//    - When responding to the MOTD message and the MOTD file
//    is found, the file is displayed line by line, with
//    each line no longer than 80 characters, using
//    RPL_MOTD format replies.  These should be surrounded
//    by a RPL_MOTDSTART (before the RPL_MOTDs) and an
//    RPL_ENDOFMOTD (after).
    RPL_ENDOFMOTD = 376,

//    "<channel> :Cannot join channel (+b)"
    ERR_BANNEDFROMCHAN=474,

//    ":[<reply>{<space><reply>}]"
//    - Reply format used by USERHOST to list replies to
//    the query list.  The reply string is composed as
//    follows:
//    <reply> ::= <nick>['*'] '=' <'+'|'-'><hostname>
//    The '*' indicates whether the client has registered
//    as an Operator.  The '-' or '+' characters represent
//    whether the client has set an AWAY message or not respectively.
    REGISTERED = 1,
    CODE_NOT_SET = 0,

    // MESSAGES

    MSG_INVITE = 1000,
    MSG_JOIN = 1001,
    MSG_GROUP_KICK = 1002,
    MSG_GROUP_PART = 1003,
    MSG_PRIVMSG = 1004,
    MSG_GROUP_NEWTOPIC = 1005,
    MSG_GROUP_MODE = 1006,
    MSG_GROUP_BAN = 1007,
    NOTICE_GROUP_INVITE = 1008,
    QUIT_MSG = 1009,

};

class Reply{
private:
    replyCodes _code;
    std::string _chan;
    std::string _receiver;
    std::string _senderNickname;
    std::string _senderUsername;
    std::string _senderHostname;
    std::string _msg;
    std::vector<std::string> _optionals;
public:
    Reply(replyCodes code,std::string chan,std::string receiver,std::string senderNickname="",std::string senderUsername="",std::string senderHostname="",std::string msg=""):\
            _code(code),_chan(chan),_receiver(receiver),_senderNickname(senderNickname), _senderUsername(senderUsername),
            _senderHostname(senderHostname),_msg(msg) {};
    ~Reply(){};
    replyCodes getCode(){
        return _code;
    }
    std::string getChan(){
        return _chan;
    }
    std::string getReceiver(){
        return _receiver;
    }
    std::string getSenderNickname(){
        return _senderNickname;
    }
    std::string getSenderUsername(){
        return _senderUsername;
    }
    std::string getSenderHostname(){
        return _senderHostname;
    }
    std::string getMsg(){
        return _msg;
    }
    void addOptional(const std::string& opt){
        _optionals.push_back(opt);
    }
    std::string getOptional(int type){
        return _optionals[type];
    }
};