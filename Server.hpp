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
    double   start_time;
    //сеторы 14
    void    setAccess(int fd);
    void    setPort(int port);
    void    setUser(User &user);
    void    setHost(std::string &host);
    void    setChannel(Channel &channel);
    void    setPassword(std::string &password);
    //геторы 30
    std::string             &getHost();
    std::string const       &getPassword(int i);
    std::vector<Channel *>  &getChannel();
    int                     &getPort(int i);
    bool                    getAccess(int fd);
    void    write_massage_to_client(int &fd, fd_set &writefds, char **buf);
    // функуии упрощающие жизнь и код 67
    void    deleteClient(int fd);
    void    deleteChannel(std::string topic);
    int     find_numb_iter(int fd);
    int     find_who_talk(fd_set &writefds);
    int     find_num_by_nickname(std::string const &nick);
    int     find_num_chan_by_name(std::string const &topic);
    int     many_or_solo_join(std::string const &arg);
    bool    can_user_talk_in_channel(int num, std::string &channel);
    //команды реализация 182
            //PRIVMSG 183
    void    privmisg_for_one_channel(int num, std::string &massage, std::string &channel);
    void    privmisg_for_one_person(int num,  std::string &name);
    void    privmisg_work(int num);
            //INFO 302
    void    info_work(int num);
            //JOIN 316
    void    say_hello_to_new_in_channel(int num, VEC_ITER_CHANEL_ADR &it_b_channel, std::string topic);
    VEC_STR &parser_of_join_chanel(std::string &msg);
    VEC_STR &parser_of_join_chanel_key(std::string &msg);
    void    create_new_channel(int num, std::string &key, std::string &topic);
    void    join_work(int num);
    bool    find_user_by_name_in_invited(std::string &name, int num_channel);
    bool    check_invite_join(int num,std::string &name_channel);
    void    join_pre_work(int num);
            //USER 537
    void    user_work(std::string &arg, int num);
    // парсер 566
    int     name_verification(std::string &buf);
    int     password_verification(std::string &buf, int num);
    void    parser_check_pas_nick_user(int num , int fd);
    void    parser_switch(int num , int fd, fd_set &writefds);
    void    parser(int num , std::string buf_str, int fd, fd_set &writefds);
    // разбивка основного цикла 712
    void    create_many_active_fd(int &fd, fd_set &activfds, int &max_d);
    void    get_new_client(int &ls, int &fd, fd_set &activfds);
    int     get_old_client_massage(int &i, fd_set &activfds, fd_set &writefds, char **buf);
    //основной цикл 788
    void    work(int ls);

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
	// int		version(int num);
	int		part(int num);
	bool	is_chan(std::string str);
	bool	chan_in_list(std::string str, std::vector<Channel *> &arr_channel);
	int		mode_chan(int num);
    int		topic(int num);
	int 	invite(int num);
	int		kick(int num);
    bool    isOper(User *usr, Channel *chan);
	int		errPrint(const int fd, std::string msg) const;
	int		rplPrint(const int fd, std::string msg) const;

public: //epilar
	bool	isNickUsed(const std::string& nickname);
	int		nick(int num, std::string& args);

	int		version(int num, std::string& args);

	int		info(int num, std::string& args);
};

#endif //IRC_SERVER_HPP
