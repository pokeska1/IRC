//
// Created by Jeannetta Marian on 12/8/21.
//

#include "Client.hpp"
Client::Client(int fd)
{
   this->fd = fd;
   this->name = "Test";
    password = false;
}

Client::Client(int fd, std::string &name)
{
    this->fd = fd;
    this->name = name;
}

int const &Client::getFd()
{
    return(this->fd);
}

void Client::setAccess(bool access)
{
    this->access = access;
}

bool const &Client::getAccess()
{
    return(this->access);
}

void Client::setPassword_init(bool password)
{
    this->password = password;
}

bool const &Client::getPassword_init()
{
    return(this->password);
}
void Client::setName_init(bool name_init)
{
    this->name_init = name_init;
}

bool const &Client::getName_init()
{
    return(this->name_init);
}

std::string const &Client::getName()
{
    return(this->name);
}



std::string const &Client::getMassage(){return(this->massage);}