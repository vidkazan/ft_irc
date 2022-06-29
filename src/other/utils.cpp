#include "../main.hpp"

void printLog(std::string description,std::string msg,std::string color){
	std::cout << color;
	if(!description.empty())
		std::cout << description << "\n";
	std::cout << "|" << msg << "|" << WHITE << "\n";
}

/**
 * @brief   Split the string using substr() function
            and adding the splitted word into the vector
 * 
 * @param   data data to split
 * @param   separator separator
 * @return  std::vector<std::string> with splitted words
 */
std::vector<std::string> split(std::string data, std::string separator) {
    std::vector<std::string>    res;
    int                         position;
    std::string                 outstr, token;

    while ((position = data.find(separator)) != std::string::npos) {
        token = data.substr(0, position);
        res.push_back(token);
        data.erase(0, position + separator.length());
    }
    // add last word
    res.push_back(data);
    return res;
}
