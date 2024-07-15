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
	std::vector<std::string> allowedMethods;
	std::vector<std::string> server_names;
	std::vector<std::string> index;
	std::string errPage;
	std::string root;
	std::string host;
	std::string maxBodySize;
	std::string proxyPass;
	std::string cgiExtentions;
	std::string cgiPath;
	std::string uploadPath;
	bool autoIndex;
	std::string location;
	std::string redirect;
	std::vector<t_servers> locations;

} t_servers;


typedef struct t_locations {
	
}t_locations;

int post(t_servers server);


#endif
