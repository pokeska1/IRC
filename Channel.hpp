#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <iostream>
#include <vector>
#include "User.hpp"

class User;
class Server;

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
	std::vector<User *>	operUsers;// channel operators
	std::vector<User *>	voteUsers;// для мода -v
	std::vector<User *>	invitedUsers;// для мода -i
	

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
	std::vector<User *> const	&getOpersVector()           const;
	std::vector<User *>	        &getInvitedVector()         ;
    std::string                 getNickname_by_it(int it)   const;
	User*				 		getOperModer();
	ModeChan					*getModeParams() { return modeParams; }
	//Setters
	void		                setTopic(std::string str);
    void		                setName(std::string str);
	void		                setPassword(std::string str);

	void		addUser(User* usr); //std::string
	void		addOperUser(User* usr); //std::string
	void		addVoteUser(User* usr); //std::string
	void		addInvitedUser(User* usr); //std::string
	void		eraseUser(User* usr);
	void		eraseOperUser(User* usr);
	void		eraseVoteUser(User* usr);
	void		eraseInvitedUser(User* usr);
	void		setOper(User* usr);
	void		removeOper(User* usr);
	std::string		setParamTrue(std::string str, std::string str_arg, std::string msg);
	std::string		setParamTrue(std::string str, std::string msg);
	std::string		setParamFalse(std::string str, std::string msg);
	User*		findUserByName(std::string str);
	User*		findUserByName(std::string str, const std::vector<User *> &v_users);
};

#endif
