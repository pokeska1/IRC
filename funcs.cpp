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
	int		info(int num)
	{
		std::string msg = "Server vesion: v1.0. Copyright 2022\n";
		write(num, msg.c_str(), msg.length() + 1);
		return 0;
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
		if ((args[0])[0] != '#' && (args[0])[0] != '&')
		{
			std::string msg = "<channel name> :No such channel\n";
			write(num, msg.c_str(), msg.length());
			return 1;
		}


	}
//*
//* Конец методов для Server.cpp
//*