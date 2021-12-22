#include "Channel.hpp"

Channel::Channel(std::string	str):name(str) {}
Channel::~Channel() {}
//Getters
std::string	Channel::getName() const { return name; }
std::string	Channel::getPassword() const { return password; }
//Setters
void		Channel::setName(std::string str) { name = str; }
void		Channel::setPassword(std::string str) { password = str; }
void		Channel::addUser(User* usr)
{
	//TODO if
	users.push_back(usr);
}
void		Channel::eraseUser(User* usr)
{
	//TODO if, iterator
	users.erase(usr);
}
void		Channel::setOper(User* usr)
{
	oper_moder = usr;
}
void		Channel::removeOper(User*)
{
	//TODO set another oper
	oper_moder = NULL;
}