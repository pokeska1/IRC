#include <vector>
#include <iostream>
#include <unistd.h>
#include "Defines.hpp"
#include "Server.hpp"

//*
//* Методы для Server.cpp
//*

//public: //rmerrie
	int		version(int num)
	{
		std::string msg = "Server vesion: v1.0\n";
		write(num, msg.c_str(), msg.length());
		return 0;
	}
	// int		info(int num)
	// {
	// 	std::string msg = "Server vesion: v1.0. Copyright 2022\n";
	// 	write(num, msg.c_str(), msg.length() + 1);
	// 	return 0;
	// }

	bool	is_chan(std::string str)
	{
		if (str[0] != '#' && str[0] != '&') //проверка: не канал
			return false;
		return true;
	}
	bool	chan_in_list(std::string str, std::vector<Channel *> &arr_channel)
	{
		std::vector<Channel *>::iterator it_chan = arr_channel.begin();
		for (; it_chan != arr_channel.end(); ++it_chan)
		{
			if (str == *it_chan)
				return true;
		}
		return false;
	}

	int		mode_chan(int num)
	{
		// this->arr_user[num] - обращение к классу User
		std::vector<std::string> args = splitStr(this->arr_user[num]->getMsgArgs());
		if (args.size() == 0) //проверка нет аргументов
		{
			std::string msg = "<command> :Not enough parameters\n";
			write(num, msg.c_str(), msg.length());
			return 1;
		}
		// if ((args[0])[0] != '#' && (args[0])[0] != '&') //проверка: не канал
		// {
		// 	std::string msg = "<channel name> :No such channel\n";
		// 	write(num, msg.c_str(), msg.length());
		// 	return 1;
		// }
		if (is_chan(args[0]) == false) //проверка: не канал
		{
			std::string msg = "<channel name> :No such channel\n";
			write(num, msg.c_str(), msg.length());
			return 1;
		}
		(args[0]).erase(0,1); // удаляем символ #/&
		// std::vector<Channel *>::iterator it_chan = arr_channel.begin();
		// for (; it_chan != arr_channel.end(); ++it_chan)
		// {
		// 	if ((args[0]) == *it_chan)
		// 		break;
		// }
		if (chan_in_list(args[0], arr_channel))
		{
			std::string msg = "<channel name> :No such channel\n";
			write(num, msg.c_str(), msg.length());
			return 1;
		}

	}
//*
//* Конец методов для Server.cpp
//*