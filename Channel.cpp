#include "Channel.hpp"

Channel::Channel(std::string str, std::string topic):name(str), topic(topic){
    modeParams = new ModeChan();
}

Channel::Channel(std::string	str):name(str)
{
	modeParams = new ModeChan();
}

Channel::~Channel()
{
	delete modeParams;
}
//Getters
std::string	Channel::getTopic() const { return topic; }
std::string	Channel::getName() const { return name; }
std::string	Channel::getPassword() const { return password; }
std::string Channel::getNickname_by_it(int it) const { return this->users[it]->getNickname(); }
std::vector<User *> const &Channel::getUsersVector() const { return this->users; }
std::vector<User *>   &Channel::getUsersVector_red(){ return(this->users);}
//Setters
void		Channel::setTopic(std::string str) { topic = str; }
void		Channel::setPassword(std::string str) { password = str; }
void		Channel::addUser(User* usr)
{

	//TODO if
	users.push_back(usr);
}
/*void		Channel::eraseUser(User* usr)
{
	//TODO if, iterator
	users.erase(usr);
}*/
void		Channel::setOper(User* usr)
{
	operModer = usr;
}
void		Channel::removeOper(User*)
{
	//TODO set another oper
	operModer = NULL;
}