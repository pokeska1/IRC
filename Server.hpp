//
// Created by Jeannetta Marian on 12/8/21.
//

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include "User.hpp"
#include "Channel.hpp"

class Server {
public:
    //основной цикл
    void work(int ls);
     // разбивка основного цикла
    void create_many_active_fd(int &fd, fd_set &activfds, int &max_d);
    void get_new_client(int &ls, int &fd, fd_set &activfds);
    void get_old_client_massage(int &i, fd_set &activfds, fd_set &writefds, char **buf);
    void write_massage_to_client(int &fd, fd_set &writefds, char **buf);
    //команды реализация
    void privmisg_work(int num);
    void info_work(int num);


    //сеторы
    void setUser(User &user);
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
    void parser(int num , std::string buf_str, int fd, fd_set &writefds);

    // функуии упрощающие жизнь и код
    int find_who_talk(fd_set &writefds);
    void deleteClient(int fd);
    int find_numb_iter(int fd);
    int find_num_by_nickname(std::string const &nick);
    unsigned int start_time;

private:
    std::vector<User *> arr_user;
    std::vector<Channel *> arr_channel; // скорее всего так будем хранить комнаты
    std::vector<int> arr_port;
    std::vector<std::string *> arr_password;
    std::string host;
	//*//std::string msgForClient;

public: //rmerrie
	//void	setMsgForClient(std::string str) { msgForClient = str; }
	std::vector<std::string>	splitStr(std::string str);
	int		version() const
	{
		//TODO How to send msg to client?
		//setMsgForClient("Server vesion: v1.0");
		std::string msg = "Server vesion: v1.0\n";
		//arr_client[num]
		//write(fd, "Server vesion: v1.0\n", msg.length() + 1); // change to msg
		return 0;
	}
};

#endif //IRC_SERVER_HPP
