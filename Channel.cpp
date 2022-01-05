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

//		########  ##     ## ######## ########  ########  #### ########
//		##     ## ###   ### ##       ##     ## ##     ##  ##  ##
//		##     ## #### #### ##       ##     ## ##     ##  ##  ##
//		########  ## ### ## ######   ########  ########   ##  ######
//		##   ##   ##     ## ##       ##   ##   ##   ##    ##  ##
//		##    ##  ##     ## ##       ##    ##  ##    ##   ##  ##
//		##     ## ##     ## ######## ##     ## ##     ## #### ########

void	Channel::setParamTrue(std::string str, std::string str_arg)
{
	for (int i = 1; i < str.length(); ++i)
	{
		if ('p' == str[i])
			modeParams->p = 1;
		else if ('s' == str[i])
			modeParams->s = 1;
		else if ('i' == str[i])
			modeParams->i = 1;
		else if ('t' == str[i])
			modeParams->t = 1;
		else if ('n' == str[i])
			modeParams->n = 1;
		else if ('m' == str[i])
			modeParams->m = 1;
		else if ('l' == str[i])
			modeParams->limit = atol(str_arg.c_str());
	}
}
void	Channel::setParamTrue(std::string str)
{
	for (int i = 1; i < str.length(); ++i)
	{
		if ('p' == str[i])
			modeParams->p = 1;
		else if ('s' == str[i])
			modeParams->s = 1;
		else if ('i' == str[i])
			modeParams->i = 1;
		else if ('t' == str[i])
			modeParams->t = 1;
		else if ('n' == str[i])
			modeParams->n = 1;
		else if ('m' == str[i])
			modeParams->m = 1;
	}
}
void	Channel::setParamFalse(std::string str)
{
	for (int i = 1; i < str.length(); ++i)
	{
		if ('p' == str[i])
			modeParams->p = 0;
		else if ('s' == str[i])
			modeParams->s = 0;
		else if ('i' == str[i])
			modeParams->i = 0;
		else if ('t' == str[i])
			modeParams->t = 0;
		else if ('n' == str[i])
			modeParams->n = 0;
		else if ('m' == str[i])
			modeParams->m = 0;
	}
}