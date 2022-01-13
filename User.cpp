#include "User.hpp"

//User::User(std::string	str):nickname(str), msg_from(NULL), msg_com(NULL),
//	msg_args(NULL), curr_buf(NULL){
//    password = false; name_init = false; access = false;
//    }
User::User(int fd):fd(fd){
//    nickname = "";msg_from = "";msg_com = "";msg_args = "";curr_buf = "";
    password = false; name_init = false; access = false; full_massage = false;
}
User::~User(){}
//Getters
int const		&User::getFd() const { return(this->fd); }
std::string	User::getNickname() const { return nickname; }
std::string	User::getPassword_network() const { return password_network; }
std::string	User::getUsername() const { return username; }
std::string	User::getHostname() const { return hostname; }
std::string	User::getServername() const { return servername; }
std::string	User::getRealname() const { return realname; }
bool const	&User::getPassword_init() const { return(this->password); }
bool const	&User::getName_init() const { return(this->name_init); }
bool const	&User::getAccess() const { return(this->access); }
bool const   &User::getFullMassage() const{ return(this->full_massage); }
std::string	&User::getMsgFrom()  { return msg_from; }
std::string	&User::getMsgCom()  { return msg_com; }
std::string	&User::getMsgArgs()  { return msg_args; }
std::vector<Channel *>  &User::getVecChannel()  { return arr_channel; }

//Setters
void		User::setNickname(std::string str) { nickname = str; }
void		User::setPassword_network(std::string str) { password_network = str; }
void		User::setUsername(std::string str) { username = str; }
void		User::setHostname(std::string str) { hostname = str; }
void		User::setServername(std::string str) { servername = str; }
void		User::setRealname(std::string str) { realname = str; }
void		User::setPassword_init(bool password) { this->password = password;}
void		User::setName_init(bool name_init) { this->name_init = name_init; }
void		User::setAccess(bool access) { this->access = access; }
void        User::setFullMassage(bool  full_massage) { this->full_massage = full_massage; }
void		User::setMsgFrom(std::string str) { msg_from = str; }
void		User::setMsgCom(std::string str) { msg_com = str; }
void		User::setMsgArgs(std::string str) { msg_args = str; }
void        User::setVecChannel(Channel  &channel)
{
    std::vector<Channel *>::iterator it_begin = arr_channel.begin();
    std::vector<Channel *>::iterator it_end = arr_channel.end();
    while (it_begin != it_end) { it_begin++; }
    arr_channel.push_back(&channel);
}

void		User::make_msg(std::string str_buf)
{
	curr_buf = str_buf;
	if (str_buf[0] == ':')
	{
		size_t	pos = curr_buf.find_first_of(' ');
		msg_from = curr_buf.substr(1, pos - 1); // записываем отправителя
		curr_buf = curr_buf.substr(pos + 1);
		while (curr_buf.find_first_not_of(" ") > 0) //удаление лишних пробелов в начале строки
			curr_buf.erase(0,1);
	}
	size_t	pos = curr_buf.find_first_of(' ');
	std::string tmp = curr_buf.substr(0, pos); //записываем имя команды для проверки
	if (matchCommand(tmp) == true)
	{
		msg_com = tmp;
		curr_buf = curr_buf.substr(pos + 1);
		if (curr_buf != "")
		{
			while (curr_buf.find_first_not_of(" ", 0) > 0) //удаление лишних пробелов в начале строки
				curr_buf.erase(0,1);
		}
	}
	else
		msg_com = "";
	if (msg_com != curr_buf)
		msg_args = curr_buf; //записываем все аргументы
}

bool		User::matchCommand(std::string str)
{
	std::string		com_array[] = { "PASS", "NICK", "USER", "OPER", "PRIVMSG",
	"NOTICE", "JOIN", "MODE", "TOPIC", "INVITE", "KICK", "PART", "KILL",
	"VERSION", "INFO", "QUIT", "ISON", "WHO", "PING" };
	unsigned len = sizeof(com_array) / sizeof(com_array[0]);
	for (unsigned i = 0; i < len; ++i)
	{
		if (str == com_array[i]) //команда валидна
			return true;
	}
	return false;
}

void		User::eraseChannel(Channel* cnl)
{
    std::vector<Channel *>::iterator	first = arr_channel.begin();
    std::vector<Channel *>::iterator	last = arr_channel.end();
    for (; first != last; ++first)
    {
        if (cnl == *first)
            arr_channel.erase(first);
    }
}

void	User::cleaner()
{
    setMsgFrom("");
    setMsgCom("");
    setMsgArgs("");
    curr_buf = "";
}