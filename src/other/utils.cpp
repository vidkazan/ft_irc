#include "../main.hpp"

void printLog(std::string description,std::string msg,std::string color){
	std::cout << color;
	if(!description.empty())
		std::cout << description << "\n";
	std::cout << "|" << msg << "|" << WHITE << "\n";
}

std::string stringToLower(std::string str){
    std::string res;
    for(int i=0;i<str.size();i++) {
        res+= toupper(str[i]);
    }
    return res;
}