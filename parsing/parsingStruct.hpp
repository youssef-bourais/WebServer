#ifndef PARSINGSTRUCT_HPP
#define PARSINGSTRUCT_HPP

#include <vector>
#include <string> // added this header

#define DEFAULT_CONF_FILE "./ConfiFile/server.conf"

typedef struct t_holder
{
	std::vector<std::string> names;
} t_holder;

typedef struct t_data
{
	std::string scopName;
	std::string scope;

	std::vector<std::string> scopes; // Locations
	std::vector<std::string> locations;
	std::vector<std::string> rulesNames;
} t_data;



#endif
