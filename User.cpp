#include "User.hpp"

User::User(std::string	str):nickname(str)
{
	msg_from = msg_com = msg_args = curr_buf = NULL;
}
User::~User() {}
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

//Setters
void		User::setNickname(std::string str) { nickname = str; }
void		User::setPassword_network(std::string str) { password_network = str; }
void		User::setUsername(std::string str) { username = str; }
void		User::setHostname(std::string str) { hostname = str; }
void		User::setServername(std::string str) { servername = str; }
void		User::setRealname(std::string str) { realname = str; }
void		User::setPassword_init(bool password) { this->password = password; }
void		User::setName_init(bool name_init) { this->name_init = name_init; }
void		User::setAccess(bool access) { this->access = access; }

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
	msg_com = curr_buf.substr(0, pos); //записываем имя команды
	unsigned len = sizeof(com_array) / sizeof(com_array[0]);
	bool flag = false;
	for (unsigned i = 0; i < len; ++i)
	{
		if (msg_com == com_array[i])
			flag = true;
	}
	if (flag == false)
		return; //нет валидной команды: чистим сообщения, выдаем ошибку в клиент
	curr_buf = curr_buf.substr(pos + 1);
	while (curr_buf.find_first_not_of(" ") > 0) //удаление лишних пробелов в начале строки
			curr_buf.erase(0,1);
	msg_args = curr_buf; //записываем все аргументы
}