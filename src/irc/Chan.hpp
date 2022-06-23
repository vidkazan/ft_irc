#pragma once
#include "../main.hpp"

class Chan {
private:
    int id;
    std::string _name;
    std::string _topic;
    std::vector<std::string> _clients;
public:
    Chan(std::string nickname, std::string name, std::string topic) : _name(name), _topic(topic) {
        _clients.push_back(nickname);
    }

    Chan(std::string nickname, std::string name) : _name(name) {
        _clients.push_back(nickname);
    }

    virtual            ~Chan() {}

    std::string getName() const { return _name; }

    void setName(std::string name) { _name = name; }

    std::string getTopic() const { return _name; }

    void setTopic(std::string topic) { _topic = topic; }

    void addClient(std::string nickname){
        for(std::vector<std::string>::iterator it = _clients.begin();it != _clients.end(); it++){
            if(nickname == *it)
                return;
        }
        std::cout << "irc: join to chan: " << _name << " by " << nickname << "\n";
        _clients.push_back(nickname);
    }
    void removeClient(std::string nickname){
        for(std::vector<std::string>::iterator it = _clients.begin();it != _clients.end(); it++){
            if(nickname == *it) {
                _clients.erase(it);
                return;
            }
        }
        // TODO remove Chan from Client obj
    }
    void setMsgToAllClients(std::string, Irc*);
};