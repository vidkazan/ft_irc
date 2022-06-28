#include "../main.hpp"

#define MIN_PORT 1024
#define MAX_PORT 49151

void validateArgv(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "need a port and a password\n";
        exit(EXIT_FAILURE);
    }
    // range check
    if (atoi(argv[1]) < MIN_PORT || atoi(argv[1]) > MAX_PORT) {
        std::cout << "not valid port\n";
        exit(EXIT_FAILURE);
    }
    // checking that all numbers
    for (int i = 0; argv[1][i] != '\0'; i++) {
        if (isdigit(i)) {
            std::cout << "not valid port\n";
            exit(EXIT_FAILURE);
        }
    }
}