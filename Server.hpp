//
// Created by Jeannetta Marian on 12/8/21.
//

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include "User.hpp"
#include "Channel.hpp"

class Server {
public:
    Server();
    //основной цикл
    void    work(int ls);
     // разбивка основного цикла
    void    create_many_active_fd(int &fd, fd_set &activfds, int &max_d);
    void    get_new_client(int &ls, int &fd, fd_set &activfds);
    int     get_old_client_massage(int &i, fd_set &activfds, fd_set &writefds, char **buf);
    void    write_massage_to_client(int &fd, fd_set &writefds, char **buf);
    //команды реализация
    void    privmisg_work(int num);
    void    info_work(int num);
    void    join_work(int num);
    void    user_work(std::string &arg, int num);

    //сеторы
    void    setUser(User &user);
    void    setChannel(Channel &channel);
    void    setPassword(std::string &password);
    void    setHost(std::string &host);
    void    setPort(int port);
    void    setAccess(int fd);
    //геторы
    int                     &getPort(int i);
    bool                    getAccess(int fd);
    std::vector<Channel *>  &getChannel(){return (this->arr_channel);}
    std::string const       &getPassword(int i);
    std::string             &getHost(){ return host; }

    // до-подобный парсер
    int     password_verification(std::string &buf, int fd, int num);
    int     name_verification(std::string &buf, int fd, int num);
    void    parser(int num , std::string buf_str, int fd, fd_set &writefds);

    // функуии упрощающие жизнь и код
    int     find_who_talk(fd_set &writefds);
    void    deleteClient(int fd);
    void    deleteChannel(std::string topic);
    int     many_or_solo_join(std::string const &arg, int num);
    int     find_numb_iter(int fd);
    int     find_num_by_nickname(std::string const &nick);
    int     find_num_chan_by_name(std::string const &topic);
    void    say_hello_to_new_in_channel(int num, std::vector<Channel *>::iterator it_b_channel, std::string topic);
    std::vector<std::string> &parser_of_join_chanel(std::string &msg);
    std::vector<std::string> &parser_of_join_chanel_key(std::string &msg);
    float   start_time;

private:
    std::vector<User *>         arr_user;
    std::vector<Channel *>      arr_channel; // скорее всего так будем хранить комнаты
    std::vector<int>            arr_port;
    std::vector<std::string *>  arr_password;
    std::string                 host;
    bool                        channel_exist; // удобный показатель есть ли хоть один канал
	//*//std::string msgForClient;

public: //rmerrie
	std::vector<std::string>	splitStr(std::string str);
	Channel						*find_chan(std::string str);
	int		version(int num);
	bool	is_chan(std::string str);
	bool	chan_in_list(std::string str, std::vector<Channel *> &arr_channel);
	int		mode_chan(int num);
    int		topic(int num);

};

#endif //IRC_SERVER_HPP
