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

    std::string getTopic() const { return _topic; }

    void setTopic(std::string topic) { _topic = topic; }

    bool addClient(std::string nickname){
        for(std::vector<std::string>::iterator it = _clients.begin();it != _clients.end(); it++){
            if(nickname == *it)
//                return 0; // already there
            return 1;
        }
        std::cout << "irc: join to chan: " << _name << " by " << nickname << "\n";
        _clients.push_back(nickname);
        return 1;
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
    bool findClient(std::string nickname){
        for(std::vector<std::string>::iterator it = _clients.begin();it != _clients.end(); it++){
            if(nickname == *it) {
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
        for(std::vector<std::string>::iterator it = _clients.begin();it != _clients.end(); it++){
            names += *it + " ";
        }
        names += "\n";
        return  names;
    }
    std::string getTopicRPL(){
        return (_name+" :"+_topic);
    }
};