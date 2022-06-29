#pragma once
#include "Client.hpp"
class Irc;
typedef struct s_chan_modes {
    bool o;
    bool i;
    bool t;
    bool b;
} t_chan_modes;

typedef struct s_user_modes {
    bool o;
} t_user_modes;

class Chan {
private:
    int id;
    std::string _name;
    std::string _topic;
    std::vector<std::pair<std::string,t_user_modes> > _clients;
    std::vector<std::string> _invitedClients;
    std::vector<std::string> _bannedClients;
    t_chan_modes _chanModes;
public:
    Chan(std::string nickname, std::string name, std::string topic) : _name(name), _topic(topic) {
        t_user_modes clientModes;
        clientModes.o = 1;
        _clients.push_back(std::make_pair(nickname, clientModes));
        _chanModes.t = 0;
        _chanModes.i = 0;
    }
    Chan(std::string nickname, std::string name) : _name(name) {
        t_user_modes clientModes;
        clientModes.o = 1;
        _clients.push_back(std::make_pair(nickname, clientModes));
        _chanModes.t = 0;
        _chanModes.i = 0;
    }

    virtual            ~Chan() {}
    std::string getName() const { return _name; }
    void setName(std::string name) { _name = name; }
    std::string getTopic() const { return _topic; }
    void setTopic(std::string topic) { _topic = topic; }
    bool addClient(std::string nickname){
        for(std::vector<std::pair<std::string,t_user_modes> >::iterator it = _clients.begin();it != _clients.end(); it++){
            if(nickname == it->first)
//                return 0; // already there
            return 1;
        }
        std::cout << "irc: join to chan: " << _name << " by " << nickname << "\n";
        t_user_modes clientModes;
        if(_clients.empty())
            clientModes.o = 1;
        else
            clientModes.o = 0;
        _clients.push_back(std::make_pair(nickname, clientModes));
        return 1;
    }
    void removeClient(std::string nickname){
        for(std::vector<std::pair<std::string,t_user_modes> >::iterator it = _clients.begin();it != _clients.end(); it++){
            if(nickname == it->first) {
                _clients.erase(it);
                return;
            }
        }
        // TODO remove Chan from Client obj
    }
    bool findClient(std::string nickname){
        for(std::vector<std::pair<std::string,t_user_modes> >::iterator it = _clients.begin();it != _clients.end(); it++){
            if(nickname == it->first) {
                return 1;
            }
        }
        return 0;
    }
    bool findOperator(std::string nickname){
        for(std::vector<std::pair<std::string,t_user_modes> >::iterator it = _clients.begin();it != _clients.end(); it++){
            if(nickname == it->first && it->second.o) {
                return 1;
            }
        }
        return 0;
    }
    void setMsgToAllClients(Irc* irc,Reply msg, std::string except="");
    int  getChanClientsCount(){return _clients.size();}
    std::string getChanNames() {
        std::string names;
        names += _name+" :";
        for(std::vector<std::pair<std::string,t_user_modes> >::iterator it = _clients.begin();it != _clients.end(); it++){
            if(it->second.o)
                names+="@";
            names += it->first + " ";
        }
        names += "\n";
        return  names;
    }
    void addClientInvite(std::string client){
        for(std::vector<std::string>::iterator it = _invitedClients.begin();it!=_invitedClients.end();it++) {
            if(client == *it) {
                return;
            }
        }
        _invitedClients.push_back(client);
    }
    void deleteClientBan(std::string client){
        for(std::vector<std::string>::iterator it = _bannedClients.begin();it!=_bannedClients.end();it++) {
            if(client == *it) {
                _bannedClients.erase(it);
                std::cout << "MODE: "<<_name<<" UNban "<<client<<"\n";
                return;
            }
        }
    }
    void addClientBan(std::string client){
        for(std::vector<std::string>::iterator it = _bannedClients.begin();it!=_bannedClients.end();it++) {
            if(client == *it) {
                return;
            }
        }
        std::cout << "MODE: "<<_name<<" ban "<<client<<"\n";
        _bannedClients.push_back(client);
    }
    bool isInviteOnly(){return(_chanModes.i);}
    bool isInvitedClient(std::string client) {
        for(std::vector<std::string>::iterator it = _invitedClients.begin();it!=_invitedClients.end();it++) {
            if(client == *it) {
                return 1;
            }
        }
        return 0;
    }
    void setChanInviteMode(char mode,int sign){
        std::cout << "MODE: "<<_name<<" set mode "<<sign<<mode<<"\n";
        if(mode == 'i' && ( sign == 0 || sign == 1))
            _chanModes.i = sign;
        if(mode == 't' && ( sign == 0 || sign == 1))
            _chanModes.t = sign;
        if(mode == 'b' && ( sign == 0 || sign == 1))
            _chanModes.b = sign;
    }
    bool checkChanClientBanned(std::string client){
        for(std::vector<std::string>::iterator it = _bannedClients.begin();it!=_bannedClients.end();it++) {
            if(client == *it) {
                return 1;
            }
        }
        return 0;
    }
};