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
    void deleteClient(int fd);
    void setPort(int port);
    void setPassword(std::string &password);
    bool getAccess(int fd);
    void setAccess(int fd);
    void setHost(std::string &host);
    int password_verification(std::string &buf, int fd);
    std::string const &getPassword(int i);
    void create_many_active_fd(int &fd, fd_set &activfds, int &max_d);
    void get_new_client(int &ls, int &fd, fd_set &activfds);
    void get_old_client_massage(int &i, fd_set &activfds, fd_set &writefds, char **buf);
    void write_massage_to_client(int &fd, fd_set &writefds, char **buf);
private:
    std::vector<Client *> arr_client;
    std::vector<int> arr_port;
    std::vector<std::string *> arr_password;
    std::string host;

public: //rmerrie
	std::vector<std::string>	splitStr(std::string str);
	int		version() const
	{
		//TODO How to send msg to client?
		std::string
		return 0;
	}
};



#endif //IRC_SERVER_HPP
