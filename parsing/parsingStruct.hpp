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

typedef struct t_servers
{
	std::vector<std::string> listen;
	std::vector<std::string> server_names;
	std::vector<std::string> host;
	std::vector<std::string> root;
	std::vector<std::string> proxyPass;
	std::vector<std::string> allowedMethods;
	std::vector<std::string> index;
	std::vector<std::string> maxBodySize;
	std::vector<std::string> autoIndex;
	std::vector<std::string> cgiExtentions;
	std::vector<std::string> uploadPath;
	std::vector<std::string> cgiPath;
	std::vector<std::string> errPage;

} t_servers;



#endif
