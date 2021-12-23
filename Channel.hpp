#pragma once

#include <iostream>
#include <vector>
#include "User.hpp"

struct ModeChan
{
	bool	p;
	bool	s;
	bool	i;
	bool	t;
	bool	n;
	bool	m;
	size_t	limit;
	//std::string	ban_mask; //is it neccessary?
	ModeChan():p(0), s(0), i(0), t(0), n(0), m(0), limit(-1) {}
	~ModeChan() {}
};


class Channel
{
private:
	std::string			name;
	std::string			password;
	User				*operModer;
	//ModeChan			&modeParams;
	std::vector<User *>	users;
	std::vector<User *>	voteUsers;

	//std::vector<int>	chan_list; //should be in Server.hpp
	Channel(const Channel &cp);
	void	operator=(const Channel &cp);
public:
	Channel(std::string	str);
	~Channel();
	//Getters
	std::string	getName() const;
	std::string	getPassword() const;
	//Setters
	void		setName(std::string str);
	void		setPassword(std::string str);

	void		addUser(User* usr); //std::string
	//void		eraseUser(User* usr);
	void		setOper(User* usr);
	void		removeOper(User* usr);
};
