#include "User.hpp"

User::User(std::string	str):nickname(str) {}
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
