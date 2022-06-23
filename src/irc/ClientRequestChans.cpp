//
// Created by Dmitrii Grigorev on 27.03.22.
//
#include "../main.hpp"

void        Client::addChannel(Chan& chan){
    for(std::vector<Chan>::iterator it = _chans.begin();it != _chans.end(); it++){
        if(chan.getName() == it->getName())
            return;
    }
    _chans.push_back(chan);
    chan.addClient(_nickname);
}
void        Client::removeChannel(Chan& chan){
    for(std::vector<Chan>::iterator it = _chans.begin();it != _chans.end(); it++){
        if(chan.getName() == it->getName()) {
            _chans.erase(it);
            chan.removeClient(_nickname);
            return;
        }
    }
}