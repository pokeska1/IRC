//
// Created by Jeannetta Marian on 12/8/21.
//

#include "Server.hpp"

void Server::Work()
{
    int ls, fd;
    int opt = 1;
    fd_set readfds, writefds;

    ls = socket(PF_INET, SOCK_STREAM, 0);
    if (ls < 0)
        exit(1);
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR,(char *)&opt, sizeof(opt));

    for(;;)
    {
        int max_d = ls;

        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(ls,&readfds);

        std::vector<Client *>::iterator it_begin = arr_client.begin();
        std::vector<Client *>::iterator it_end = arr_client.end();
        for(fd = (*it_begin)->getFd(); fd != (*it_end)->getFd(); fd = (*it_begin++)->getFd())
        {
            FD_SET(fd, &readfds);
            if ((*it_begin)->getMassage() != "")
                FD_SET(fd, &writefds);
            if (fd > max_d)
                max_d = fd;
        }
        int res = select(max_d+1, &readfds, NULL,NULL, NULL);
        if (res < 1){
            if (errno != EINTR){
                std::cout << "error in select";
            }
            else {
                std::cout << "signal error";
            }
            continue;
        }
        if (res == 0){
            continue;
        }
//        if (FD_ISSET(ls, &readfds)){
//            accept(fd, )
//        }
    }
}