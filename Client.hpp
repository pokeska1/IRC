//
// Created by Jeannetta Marian on 12/8/21.
//

#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <vector>
#include <iostream>
#include <string>


class Client {
public:
    Client(int fd);
    int const &getFd();
    std::string const &getMassage();
private:
    int fd;
    std::string massage;
};


#endif //IRC_CLIENT_HPP
