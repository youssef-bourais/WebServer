#ifndef PARSE_HPP
#define PARSE_HPP
#define DEFAULT_CONF_FILE "server.conf"

#include "Parsing.hpp"

bool parse(int ac, char **av);
void serv(std::vector<t_data> data, Parsing &parse);

#endif