#include <iostream>
#include <vector>
#include <unistd.h>

std::vector<std::string>	splitStr(std::string str)
{
	std::vector<std::string>	res;
	std::string delimiter = " ";
	size_t pos = 0;
	std::string tmp;

	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		tmp = str.substr(0, pos);
		res.push_back(tmp); //action
		str.erase(0, pos + delimiter.length());
		while (str.find_first_not_of(" ") > 0) //удаление лишних пробелов в начале строки
			str.erase(0,1);
	}
	tmp = str.substr(0, pos);
	res.push_back(tmp); //action
	return res;
}

int	main()
{
	std::string str = "Some     string   with    spases" ;
	std::vector<std::string>	splited;
	splited = splitStr(str);
	std::cout << "Size:" << splited.size() << std::endl;
	for (size_t i = 0; i < splited.size(); ++i)
	{
		std::cout << splited[i] << std::endl;
	}
	write(1, str.c_str(), str.length());
	return 0;
}