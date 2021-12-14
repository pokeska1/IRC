//
// Created by Jeannetta Marian on 12/8/21.
//

#include "Server.hpp"

void Server::setClient(Client &client)
{
    std::vector<Client *>::iterator it_begin = arr_client.begin();
    std::vector<Client *>::iterator it_end = arr_client.end();
    while (it_begin != it_end)
    {
        it_begin++;
    }
    arr_client.push_back(&client);

}

void Server::work(int ls)
{
    struct sockaddr_in clientfd;
    char buf[BUFLEN];
    socklen_t size;
    //int fcntl_ret = fcntl(ls, F_SETFL, O_NONBLOCK);
//    struct sockaddr_in addr;
//    addr.sin_family = AF_INET;
//    addr.sin_port = htons(PORT);
//    addr.sin_addr.s_addr = htonl(INADDR_ANY);
//
    int max_d = ls; // считаем что макс сокет это нынешний слушающий
    for(;;)
    {
        int fd;
        fd_set readfds, writefds;




        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(ls, &readfds); //вводим fd слушашего сокета в множество


        std::vector<Client *>::iterator it_begin = arr_client.begin();
        std::vector<Client *>::iterator it_end = arr_client.end();

        // а теперь цикл по сокетам
        if (it_begin !=  it_end) {
            fd = (*it_begin)->getFd();
            while (it_begin != it_end)
            {
                fd = (*it_begin)->getFd();
                FD_SET(fd, &readfds);
                if (fd > max_d)
                    max_d = fd;

                it_begin++;
            }
        }

        int res = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);

        if(res < 1)
            if (errno != EINTR){
                perror("Server: cannot creat socket");
                exit(EXIT_FAILURE);
            }
//        else {
//                //perror("Server: meh");
//            }
        for(int i = 0 ; i <= max_d; i++) {
            if (i == ls) {
                if (FD_ISSET(ls, &readfds)) {
                    size = sizeof(clientfd);
                    fd = accept(ls, (struct sockaddr *) &clientfd, &size);
                    if (fd < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "\"Server: connect from host " << inet_ntoa(clientfd.sin_addr)
                    << ", port " << ntohs(clientfd.sin_port) <<"." << std::endl;
                    FD_SET(fd, &readfds);
                    Client *new_client = new Client(fd);
                    this->setClient(*new_client);
                }
            }
             else {
                if (FD_ISSET(i, &readfds))
                {
                    int nbytes;

                   //nbytes = read(fd, buf, 512);
                    nbytes = recv(i, buf, 512, 0);
                    if (nbytes < 0)
                    {
                        perror("Server: meh nbytes < 0");
                    }
                    else if (nbytes == 0){
                        perror("Server: meh nbytes == 0");
                    }
                    else{
                        std::cout << "Serv got massage: " << buf << std::endl;
                        for(int i = 0; i != BUFLEN; ++i)
                            buf[i] = '\0';
                        FD_CLR(i, &readfds);
                    }
                }
            }
        }

        std::vector<Client *>::iterator it_begin_new = arr_client.begin();
        std::vector<Client *>::iterator it_end_new = arr_client.end();
        //fd = (* it_begin_new)->getFd();
        while (it_begin_new != it_end_new)
            {
                fd = (*it_begin_new)->getFd();
           if (FD_ISSET(fd, &readfds))
           {
               int nbytes;

            //   nbytes = read(fd, buf, 512);
                nbytes = recv(fd, buf, 512, 0);
               if (nbytes < 0)
               {
                   perror("Server: meh nbytes < 0");
               }
               else if (nbytes == 0){
                   perror("Server: meh nbytes == 0");
               }
               else{
                   std::cout << "Serv got massage: " << buf << std::endl;
                   //fprintf(stdout, "Serv got massage: %s/n", buf);
                   for(int i = 0; i != BUFLEN; ++i)
                    buf[i] = '\0';
                   FD_CLR(fd, &readfds);
               }
           }
           if (FD_ISSET(fd, &writefds))
           {
               int nbytes;
               unsigned char *s;

               for (s = (unsigned char *)buf; *s ;s++) *s = toupper(*s);
               nbytes = write(fd, buf, strlen(buf) + 1);
               fprintf(stdout, "Write back: %s/nbytes=%d/n", buf, nbytes);
               for(int i = 0; i != BUFLEN; ++i)
                   buf[i] = '\0';
               if(nbytes < 0){
                   perror("Server: write failure");
               }
           }
                it_begin_new++;
       }
    }
}