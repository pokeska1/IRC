#pragma once

#include <iostream>
#include <vector>
#include "User.hpp"

class Channel
{
private:
	std::string			name;
	std::string			password;
	User 				*oper_moder;
	std::vector<User *>	users;

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
	void		eraseUser(User* usr);
	void		setOper(User* usr);
	void		removeOper(User* usr);
};
