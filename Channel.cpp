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
void		Channel::eraseUser(User* usr)
{
	std::vector<User *>::iterator	first = users.begin();
	std::vector<User *>::iterator	last = users.end();
	for (; first != last; ++first)
	{
		if (usr == *first)
			users.erase(first);
	}
}
void		Channel::addVoteUser(User* usr)
{
	//TODO if
	if (voteUsers.empty())
		voteUsers.push_back(operModer);
	voteUsers.push_back(usr);
}
void		Channel::eraseVoteUser(User* usr)
{
	std::vector<User *>::iterator	first = voteUsers.begin();
	std::vector<User *>::iterator	last = voteUsers.end();
	for (; first != last; ++first)
	{
		if (usr == *first)
			voteUsers.erase(first);
	}
}
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

User*		Channel::findUserByName(std::string str)
{
	for (int i = 0; i < users.size(); ++i)
	{
		if (users[i]->getNickname() == str)
			return users[i];
	}
	return NULL;
}

void	Channel::setParamTrue(std::string flags, std::string flag_arg)
{
	for (int i = 1; i < flags.length(); ++i)
	{
		if ('p' == flags[i])
			modeParams->p = 1;
		else if ('s' == flags[i])
			modeParams->s = 1;
		else if ('i' == flags[i])
			modeParams->i = 1;
		else if ('t' == flags[i])
			modeParams->t = 1;
		else if ('n' == flags[i])
			modeParams->n = 1;
		else if ('m' == flags[i])
			modeParams->m = 1;
		else if ('l' == flags[i])//add check of number
			modeParams->limit = atol(flag_arg.c_str());
		else if ('o' == flags[i])
		{
			User * new_oper = findUserByName(flag_arg);
			if (new_oper != NULL)
				setOper(new_oper);
		}
		else if ('v' == flags[i])
		{
			User * user_to_add = findUserByName(flag_arg);
			if (user_to_add != NULL)
				addVoteUser(user_to_add);
		}
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