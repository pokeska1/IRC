//
// Created by Jeannetta Marian on 12/8/21.
//

#include "Client.hpp"
Client::Client(int fd):fd(fd){}
int const &Client::getFd(){return(this->fd);}
std::string const &Client::getMassage(){return(this->massage);}