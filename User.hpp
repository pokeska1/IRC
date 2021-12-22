#pragma once

#include <iostream>
#include <vector>

class User
{
private:
	std::string			nickname;
	std::string			password;
	std::string			username;
	std::string			hostname;
	std::string			servername;
	std::string			realname;

	User(const User &cp);
	void	operator=(const User &cp);
public:
	User(std::string	str);
	~User();
	//Getters
	std::string	getNickname() const;
	std::string	getPassword() const;
	std::string	getUsername() const;
	std::string	getHostname() const;
	std::string	getServername() const;
	std::string	getRealname() const;
	//Setters
	void		setNickname(std::string str);
	void		setPassword(std::string str);
	void		setUsername(std::string str);
	void		setHostname(std::string str);
	void		setServername(std::string str);
	void		setRealname(std::string str);
};
