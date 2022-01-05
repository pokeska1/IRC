#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <iostream>
#include <vector>
#include "User.hpp"

class User;

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
	std::string			topic;
    std::string			name;
	std::string			password;
	User				*operModer;
	ModeChan			*modeParams;
	std::vector<User *>	users;
	std::vector<User *>	voteUsers;// для мода -v

	//std::vector<int>	chan_list; //should be in Server.hpp
	Channel(const Channel &cp);
	void	operator=(const Channel &cp);
public:
	Channel(std::string	str);
    Channel(std::string	str, std::string topic);

	~Channel();
	//Getters
	std::string	                getTopic()                  const;
    std::string	                getName()                   const;
	std::string	                getPassword()               const;
    std::vector<User *> const	&getUsersVector()           const;
    std::vector<User *>         &getUsersVector_red();//
    std::string                 getNickname_by_it(int it)   const;
	//Setters
	void		                setTopic(std::string str);
    void		                setName(std::string str);
	void		                setPassword(std::string str);

	void		addUser(User* usr); //std::string
//	void		eraseUser(User* usr);
	void		setOper(User* usr);
	void		removeOper(User* usr);
	void		setParamTrue(std::string str, std::string str_arg);
	void		setParamTrue(std::string str);
	void		setParamFalse(std::string str);
};

#endif
