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
#include <fcntl.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <vector>
#include <iostream>
#include <string>
#include <map>


#define BUFLEN 512
#define PORT 5555


class Client {
public:
    Client(int fd);
    Client(int fd, std::string &name);


    int const &getFd();
    std::string const &getName();
    std::string const &getMassage();
    bool const &getAccess();
    bool const &getPassword_init();
    bool const &getName_init();


    void setName(std::string const &name);
    void setAccess(bool access);
    void setPassword_init(bool password);
    void setName_init(bool name_init);

private:
    int fd;
    std::string massage;
    std::string name;
    bool password;
    bool name_init;
    bool access;
};


#endif //IRC_CLIENT_HPP
