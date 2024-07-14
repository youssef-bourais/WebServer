#include "./Parsing.hpp"
#include "parse.hpp"
// #include "../server/Server.hpp"
#include "../errors/Errors.hpp"

bool parse(int ac, char **av)
{
	ErrorsChecker checker;
	std::string filePath;
	std::vector<t_data> data;
	std::vector<t_data>::iterator itr;
	std::vector<std::string> holder;
	std::string loc;
	std::vector<std::string>::iterator holderItr;

	if (ac == 1)
		filePath = DEFAULT_CONF_FILE;
	else
		filePath = av[1];

	Parsing parsing(filePath);

	data = parsing.getData();

	return true;
}
