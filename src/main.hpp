#pragma once
#include <fcntl.h>
#include <iostream>

#define SOME "\e[38m"
#define RED "\e[91m"
#define YELLOW "\e[93m"
#define GREEN "\e[92m"
#define WHITE "\e[39m"

void        printLog(std::string description,std::string msg,std::string color);
void        validateArgv(int argc, char **argv);