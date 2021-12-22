//
// Created by Jeannetta Marian on 12/8/21.
//

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include "Client.hpp"


class Server {
public:
    //основной цикл
    void work(int ls);
     // разбивка основного цикла
    void create_many_active_fd(int &fd, fd_set &activfds, int &max_d);
    void get_new_client(int &ls, int &fd, fd_set &activfds);
    void get_old_client_massage(int &i, fd_set &activfds, fd_set &writefds, char **buf);
    void write_massage_to_client(int &fd, fd_set &writefds, char **buf);
    void privmisg_work(int num);


    //сеторы
    void setClient(Client &client);
    void setPassword(std::string &password);
    void setHost(std::string &host);
    void setPort(int port);
    //геторы
    int &getPort(int i);
    bool getAccess(int fd);
    void setAccess(int fd);
    std::string const &getPassword(int i);
    // до-подобный парсер
    int password_verification(std::string &buf, int fd, int num);
    int name_verification(std::string &buf, int fd);
    //реалтзация команд
    void parser(int num , std::string buf_str, int fd, std::string command, std::string massage);

    // функуии упрощающие жизнь и код
    int find_who_talk(fd_set &writefds);
    void deleteClient(int fd);
    int find_numb_iter(int fd);

private:
    std::vector<Client *> arr_client;
    std::vector<int> arr_port;
    std::vector<std::string *> arr_password;
    std::string host;
};



#endif //IRC_SERVER_HPP
