#include "Parsing.hpp"
#include <sstream>
#include "../Tools/Tools.hpp"

void Parsing::checkUnknownKey(std::vector<t_data> data)
{
	std::vector<std::string>::iterator itr;
	std::vector<t_data>::iterator dataItr;
	std::vector<std::string> names;
	names.push_back("listen");
	names.push_back("server_names");
	names.push_back("host");
	names.push_back("root");
	names.push_back("proxy_pass");
	names.push_back("allowed_methods");
	names.push_back("index");
	names.push_back("maxBodySize");
	names.push_back("upload_path");
	names.push_back("autoIndex");
	names.push_back("cgi_extentions");
	names.push_back("upload_path");
	names.push_back("cgi_path");
	names.push_back("error_page");
	dataItr = data.begin();
	while (dataItr != data.end())
	{
		itr = dataItr->rulesNames.begin();
		while (itr != dataItr->rulesNames.end())
		{
			if (std::find(names.begin(), names.end(), *itr) == names.end())
			{
				std::string error = "\x1b[31mError: Unknown rule " + *itr + ".";
				throw std::runtime_error(error.c_str());
			}
			*itr++;
		}
		*dataItr++;
	}
}

void Parsing::checkPortNumber(std::vector<std::string> data, size_t counter)
{
	std::string err;
	if (data.size() == 0)
	{
		return;
		// err = "\x1b[31mError: Server " + intToString(counter) + " don't have a port to listen to.";
		// throw std::runtime_error(err);
	}
	else if (data.size() > 1)
	{
		err = "\x1b[31mError: Server " + intToString(counter) + " can't listen to multiple port.";
		throw std::runtime_error(err);
	}
	else if (data[0].find_first_not_of("0987654321") != std::string::npos)
	{
		err = "\x1b[31mError: Server " + intToString(counter) + " don't have a valid port number.";
		throw std::runtime_error(err);
	}
	else if (data[0].length() > 4)
	{
		err = "\x1b[31mError: Server " + intToString(counter) + " don't have a valid range port number.";
		throw std::runtime_error(err);
	}
}

void Parsing::checkDuplicatedPort(std::vector<std::string> ports, std::string port)
{
	if (ports.size() == 0)
		return;
	if (std::find(ports.begin(), ports.end(), port) != ports.end())
		throw std::runtime_error("\x1b[31mError: Duplicated ports between servers.");
}

#include <cstdlib>  // for atoi

void Parsing::checkHost(std::vector<std::string> data, size_t counter)
{
	std::string err;
	std::vector<std::string> holder;
	std::vector<std::string>::iterator itr;

	if (data.size() == 0)
	{
		err = "\x1b[31mError: Server " + intToString(counter) + " don't have a host to listen to.";
		throw std::runtime_error(err);
	}
	else if (data.size() > 1)
	{
		err = "\x1b[31mError: Server " + intToString(counter) + " can't listen to multiple hosts.";
		throw std::runtime_error(err);
	}
	else if (data[0].find_first_not_of("0987654321.") != std::string::npos || countAppearance(data[0], '.') != 3)
	{
		err = "\x1b[31mError: Server " + intToString(counter) + " don't have a valid host ip";
		throw std::runtime_error(err);
	}
	for (int idx = 0; idx < 4; idx++)
	{
		if (data[0].substr(0, data[0].find_first_of(".")).length() == 0)
		{
			err = "\x1b[31mError: Server " + intToString(counter) + " don't have a valid host ip (empty digits).";
			throw std::runtime_error(err);
		}
        else if (atoi(data[0].substr(0, data[0].find_first_of(".")).c_str()) > 255) 
		{
			err = "\x1b[31mError: Server " + intToString(counter) + " don't have a valid host ip (out range digits).";
			throw std::runtime_error(err);
		}
		data[0] = data[0].erase(0, data[0].find_first_of(".") + 1);
	}
}

void Parsing::checkDuplicatedHosts(std::vector<std::string> data, std::string value)
{
	if (std::find(data.begin(), data.end(), value) != data.end())
		throw std::runtime_error("\x1b[31mError: Duplicated hosts between servers.");
}

void Parsing::checkAllowedMethods(std::vector<std::string> data, size_t counter)
{
	std::vector<std::string>::iterator itr;
	std::string err;

	if (data.size() == 0)
	{
		err = "\x1b[31mError: Server " + intToString(counter) + " don't have a specified HTTP methods";
		throw std::runtime_error(err);
	}
	itr = data.begin();
	while (itr != data.end())
	{
		if (itr->compare("POST") != 0 && itr->compare("GET") != 0 && itr->compare("DELETE") != 0)
		{
			err = "\x1b[31mError: Server " + intToString(counter) + " Unknowd method " + *itr;
			throw std::runtime_error(err);
		}
		*itr++;
	}
}
// For now ....
void Parsing::checkRepeatedMethods(std::vector<std::string> data, size_t counter)
{
	std::vector<std::string>::iterator itr;
	std::vector<std::string> collected;
	std::string err;

	itr = data.begin();
	while (itr != data.end())
	{
		if (std::find(collected.begin(), collected.end(), *itr) != collected.end())
		{
			err = "\x1b[31mError: Server " + intToString(counter) + " repeated HTTP mthods " + *itr;
			;
			throw std::runtime_error(err);
		}
		collected.push_back(*itr);
		*itr++;
	}
}

void Parsing::checkBodySize(std::vector<std::string> vec, size_t counter)
{
	std::string err;
	std::vector<std::string>::iterator itr;
	if (vec.size() == 0)
	{
		return;
		// err = "\x1b[31mError: Server " + intToString(counter) + " don't have a specific bodySize.";
		// throw std::runtime_error(err);
	}
	else if (vec.size() != 1)
	{
		err = "\x1b[31mError: Server " + intToString(counter) + " should only have one maxBodySize number.";
		throw std::runtime_error(err);
	}
	itr = vec.begin();
	if (itr->find_first_not_of("0987654321") != std::string::npos)
	{
		err = "\x1b[31mError: Server " + intToString(counter) + " have an invalid body size value.";
		throw std::runtime_error(err);
	}
}

void Parsing::checkForErrors(std::vector<t_data> data)
{
	std::vector<t_data>::iterator itr;
	std::vector<std::string> portsHolder;
	std::vector<std::string> hostsHolder;
	std::vector<std::string> holder;

	checkUnknownKey(data);
	size_t counter = 1;
	itr = data.begin();
	while (itr != data.end())
	{
		holder = getRule(*itr, "listen");
		if (holder.size() > 0)
		{
			checkPortNumber(getRule(*itr, "listen"), counter);
			checkDuplicatedPort(portsHolder, getRule(*itr, "listen")[0]);
			portsHolder.push_back(getRule(*itr, "listen")[0]);
		}
		holder = getRule(*itr, "host");
		if (holder.size() > 0)
		{
			checkHost(getRule(*itr, "host"), counter);
			checkDuplicatedHosts(hostsHolder, getRule(*itr, "host")[0]);
			hostsHolder.push_back(getRule(*itr, "host")[0]);
		}
		holder = getRule(*itr, "allowed_methods");
		if (holder.size() > 0)
		{
			checkAllowedMethods(getRule(*itr, "allowed_methods"), counter);
			checkRepeatedMethods(getRule(*itr, "allowed_methods"), counter);
		}
		holder = getRule(*itr, "maxBodySize");
		if (holder.size() > 0)
			checkBodySize(getRule(*itr, "maxBodySize"), counter);

		counter++;
		*itr++;
	}
}
