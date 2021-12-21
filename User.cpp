#include "User.hpp"

User::User(std::string	str):nickname(str) {}
User::~User() {}
//Getters
std::string	User::getNickname() const { return nickname; }
std::string	User::getPassword() const { return password; }
std::string	User::getUsername() const { return username; }
std::string	User::getHostname() const { return hostname; }
std::string	User::getServername() const { return servername; }
std::string	User::getRealname() const { return realname; }
//Setters
void		User::setNickname(std::string str) { nickname = str; }
void		User::setPassword(std::string str) { password = str; }
void		User::setUsername(std::string str) { username = str; }
void		User::setHostname(std::string str) { hostname = str; }
void		User::setServername(std::string str) { servername = str; }
void		User::setRealname(std::string str) { realname = str; }