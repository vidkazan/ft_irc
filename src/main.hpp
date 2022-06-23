#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <dirent.h>

#define SOME "\e[38m"
#define RED "\e[91m"
#define YELLOW "\e[93m"
#define GREEN "\e[92m"
#define WHITE "\e[39m"

#define READING 0
#define WRITING 1
#define CLOSING 2

#include "irc/Request.hpp"
#include "irc/Response.hpp"
#include "irc/Client.hpp"
#include "irc/Chan.hpp"
#include "server/PortServer.hpp"
#include "server/Irc.hpp"

void printLog(std::string description,std::string msg,std::string color);
