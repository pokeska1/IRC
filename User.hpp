#pragma once

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
#include <vector>
#include <iostream>
#include <string>


#define BUFLEN 512
#define PORT 5555

class User
{
private:
	int				fd;
	std::string		nickname;
	std::string		password_network;
	std::string		username;
	std::string		hostname;
	std::string		servername;
	std::string		realname;
	bool			password;
	bool			name_init;
	bool			access;

	User(const User &cp);
	void	operator=(const User &cp);
public:
	User(std::string	str);
	~User();
	//Getters
	int	const	&getFd() const;
	std::string	getNickname() const;
	std::string	getPassword_network() const;
	std::string	getUsername() const;
	std::string	getHostname() const;
	std::string	getServername() const;
	std::string	getRealname() const;
	bool const	&getPassword_init() const;
	bool const	&getName_init() const;
	bool const	&getAccess() const;
	//Setters
	void		setNickname(std::string str);
	void		setPassword_network(std::string str);
	void		setUsername(std::string str);
	void		setHostname(std::string str);
	void		setServername(std::string str);
	void		setRealname(std::string str);
	void		setPassword_init(bool password);
	void		setName_init(bool name_init);
	void		setAccess(bool access);
};