#pragma once
#include "../main.hpp"

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
        clientModes.b = 0;
        _clients.push_back(std::make_pair(nickname, clientModes));
        _chanModes.b = 1;
        _chanModes.o = 1;
        _chanModes.t = 1;
        _chanModes.i = 0;
    }

    Chan(std::string nickname, std::string name) : _name(name) {
        t_user_modes clientModes;
        clientModes.o = 1;
        clientModes.b = 0;
        _clients.push_back(std::make_pair(nickname, clientModes));
        _chanModes.b = 1;
        _chanModes.o = 1;
        _chanModes.t = 1;
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
        clientModes.o = 0;
        clientModes.b = 0;
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

    void setMsgToAllClients(std::string ,std::string, Irc*);
    void setMsgToAllClients(std::string, Irc*);

    int  getChanClientsCount(){return _clients.size();}
    std::string getChanNames() {
        std::string names;
        names += _name+" : ";
        for(std::vector<std::pair<std::string,t_user_modes> >::iterator it = _clients.begin();it != _clients.end(); it++){
            if(it->second.o)
                names+="@";
            names += it->first + " ";
        }
        names += "\n";
        return  names;
    }
    std::string getTopicRPL(){
        return (_name+" :"+_topic);
    }
};