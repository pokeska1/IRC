#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <vector>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <map>
#include <vector>

#include "Defines.hpp"
#include "Channel.hpp"

#define BUFLEN 512
#define PORT 5555

class Channel;

class User
{
private:
	int				        fd;
	std::string		        nickname;
	std::string		        password_network;
	std::string		        username;
	std::string		        hostname;
	std::string		        servername;
	std::string		        realname;
	bool			        password;
	bool			        name_init;
	bool			        access;
    bool                    full_massage;

	std::string		        msg_from;
	std::string		        msg_com;
	std::string		        msg_args;
	std::string		        curr_buf;
    std::vector<Channel *>  arr_channel;


	User(const User &cp);
	void	operator=(const User &cp);
public:
    User(int            fd);
//	User(std::string	str);
	~User();
	//Getters
	int	const    &getFd()                const;
	std::string  getNickname()           const;
	std::string  getPassword_network()   const;
	std::string  getUsername()           const;
	std::string  getHostname()           const;
	std::string  getServername()         const;
	std::string  getRealname()           const;
	bool const   &getPassword_init()     const;
	bool const   &getName_init()         const;
	bool const   &getAccess()            const;
    bool const   &getFullMassage()       const;
    std::string  &getMsgFrom();
    std::string  &getMsgCom();
    std::string  &getMsgArgs();
    std::vector<Channel *>  &getVecChannel();
	//Setters
	void    setNickname(std::string str);
	void    setPassword_network(std::string str);
	void    setUsername(std::string str);
	void    setHostname(std::string str);
	void    setServername(std::string   str);
	void    setRealname(std::string str);
	void    setPassword_init(bool   password);
	void    setName_init(bool   name_init);
	void    setAccess(bool  access);
    void    setFullMassage(bool  full_massage);
    void    setMsgFrom(std::string str);
    void    setMsgCom(std::string str);
    void    setMsgArgs(std::string str);
    void    setVecChannel(Channel &channel);

    void    make_msg(std::string str_buf);
    void    delete_channel();
    void	eraseChannel(Channel* cnl);
	bool    matchCommand(std::string str);
    void    cleaner();
};

#endif
