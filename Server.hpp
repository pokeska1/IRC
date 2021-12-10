//
// Created by Jeannetta Marian on 12/8/21.
//

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include "Client.hpp"


class Server {

public:
    void work(int ls);
    void setClient(Client &client);
private:
    std::vector<Client *> arr_client;


};


#endif //IRC_SERVER_HPP
