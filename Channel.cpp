#include "Channel.hpp"

Channel::Channel(std::string str, std::string topic):topic(topic), name(str){
    modeParams = new ModeChan();
}

Channel::Channel(std::string	str):topic(""), name(str)
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
std::vector<User *> const &Channel::getOpersVector() const { return this->operUsers; }
std::vector<User *> &Channel::getInvitedVector() { return this->invitedUsers; }
//Setters
void		Channel::setTopic(std::string str) { topic = str; }
void		Channel::setPassword(std::string str) { password = str; }
void		Channel::addUser(User* usr)
{
	std::vector<User *>::iterator	it;
	it = find(users.begin(), users.end(), usr);
	if (it == users.end())
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
void		Channel::addOperUser(User* usr)
{
	std::vector<User *>::iterator	it;
	it = find(operUsers.begin(), operUsers.end(), usr);
	if (it == operUsers.end())
		operUsers.push_back(usr);
}
void		Channel::addVoteUser(User* usr)
{
	if (voteUsers.empty())
		voteUsers.push_back(operModer);
	std::vector<User *>::iterator	it;
	it = find(voteUsers.begin(), voteUsers.end(), usr);
	if (it == voteUsers.end())
		voteUsers.push_back(usr);
}
void		Channel::addInvitedUser(User* usr)
{
	std::vector<User *>::iterator	it;
	it = find(invitedUsers.begin(), invitedUsers.end(), usr);
	if (it == invitedUsers.end())
		invitedUsers.push_back(usr);
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
void		Channel::eraseOperUser(User* usr)
{
	std::vector<User *>::iterator	first = operUsers.begin();
	std::vector<User *>::iterator	last = operUsers.end();
	for (; first != last; ++first)
	{
		if (usr == *first)
			operUsers.erase(first);
	}
}
void		Channel::eraseInvitedUser(User* usr)
{
	std::vector<User *>::iterator	first = invitedUsers.begin();
	std::vector<User *>::iterator	last = invitedUsers.end();
	for (; first != last; ++first)
	{
		if (usr == *first)
			invitedUsers.erase(first);
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

User*		Channel::getOperModer() { return operModer; }

User*		Channel::findUserByName(std::string str)
{
	for (int i = 0; (unsigned long)i < users.size(); ++i)
	{
		if (users[i]->getNickname() == str)
			return users[i];
	}
	return NULL;
}

User*		Channel::findUserByName(std::string str, const std::vector<User *> &v_users)
{
	for (int i = 0; (unsigned long)i < v_users.size(); ++i)
	{
		if (v_users[i]->getNickname() == str)
			return users[i];
	}
	return NULL;
}

std::string	Channel::setParamTrue(std::string flags, std::string flag_arg, std::string msg)
{
	msg += " +";
	for (int i = 0; (unsigned long)i < flags.length(); ++i)
	{
		if ('p' == flags[i])
		{
			modeParams->p = 1;
			msg += "p";
		}
		else if ('s' == flags[i])
		{
			modeParams->s = 1;
				msg += "s";
		}
		else if ('i' == flags[i])
		{
			modeParams->i = 1;
				msg += "i";
		}
		else if ('t' == flags[i])
		{
			modeParams->t = 1;
			msg += "t";
		}
		else if ('n' == flags[i])
		{
			modeParams->n = 1;
				msg += "n";
		}
		else if ('m' == flags[i])
		{
			modeParams->m = 1;
				msg += "m";
		}
		else if ('l' == flags[i])
		{
			modeParams->limit = atol(flag_arg.c_str());
			msg += "l ";
			msg += std::to_string(getModeParams()->limit);
		}
		else if ('o' == flags[i])
		{
			User * new_oper = findUserByName(flag_arg);
			if (new_oper != NULL)
			{
				addOperUser(new_oper);
				msg += "o ";
				msg += flag_arg;
			}
			else
				msg = "MSG_NOSUCHNICK";
		}
		else if ('v' == flags[i])
		{
			User * user_to_add = findUserByName(flag_arg);
			if (user_to_add != NULL)
			{
				addVoteUser(user_to_add);
				msg += "v ";
				msg += flag_arg;
			}
			else
				msg = "MSG_NOSUCHNICK";
		}
		else if ('k' == flags[i])
			setPassword(flag_arg);
	}
	msg += "\r\n";
	return msg;
}
std::string	Channel::setParamTrue(std::string str, std::string msg)
{
	msg += " +";
	for (int i = 0; (unsigned long)i < str.length(); ++i)
	{
		if ('p' == str[i])
		{
			modeParams->p = 1;
			msg += "p";
		}
		else if ('s' == str[i])
		{
			modeParams->s = 1;
				msg += "s";
		}
		else if ('i' == str[i])
		{
			modeParams->i = 1;
				msg += "i";
		}
		else if ('t' == str[i])
		{
			modeParams->t = 1;
			msg += "t";
		}
		else if ('n' == str[i])
		{
			modeParams->n = 1;
				msg += "n";
		}
		else if ('m' == str[i])
		{
			modeParams->m = 1;
				msg += "m";
		}
	}
	msg += "\r\n";
	return msg;
}
std::string	Channel::setParamFalse(std::string str, std::string msg)
{
	msg += " -";
	for (int i = 0; (unsigned long)i < str.length(); ++i)
	{
		if ('p' == str[i])
		{
			modeParams->p = 0;
			msg += "p";
		}
		else if ('s' == str[i])
		{
			modeParams->s = 0;
				msg += "s";
		}
		else if ('i' == str[i])
		{
			modeParams->i = 0;
				msg += "i";
		}
		else if ('t' == str[i])
		{
			modeParams->t = 0;
			msg += "t";
		}
		else if ('n' == str[i])
		{
			modeParams->n = 0;
				msg += "n";
		}
		else if ('m' == str[i])
		{
			modeParams->m = 0;
				msg += "m";
		}
	}
	msg += "\r\n";
	return msg;
}
