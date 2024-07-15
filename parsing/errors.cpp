#include "Parsing.hpp"
#include <sstream>
#include "../Tools/Tools.hpp"

void Parsing::checkUnknownKey(std::vector<t_data> data)
{
	std::vector<std::string>::iterator itr;
	std::vector<t_data>::iterator dataItr;
	std::vector<std::string> names;
	names.push_back("listen"); //? Port numbers
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
	names.push_back("redirect");
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

// NOTE Ports checking

void Parsing::checkPortNumber(std::vector<std::string> data, size_t counter)
{
	std::vector<std::string>::iterator itr;
	std::string err;

	itr = data.begin();
	if (data.size() == 0) {
		err = "\x1b[31mError: Server " + intToString(counter) + " has no specified ports.";
		throw std::runtime_error(err);
	}
	while (itr != data.end()) {
		if (itr->length() == 0)
		{
			itr++;
			continue;
		}
		if (checkDuplicatedValues(data)) {
			err = "\x1b[31mError: Server " + intToString(counter) + " has a duplicated ports";
			throw std::runtime_error(err);
		}
		if (itr->find_first_not_of("0987654321") != std::string::npos) {
			err = "\x1b[31mError: Server " + intToString(counter) + " don't have a valid port number.";
			throw std::runtime_error(err);
		}
		if (itr->length() > 4) {
			err = "\x1b[31mError: Server " + intToString(counter) + " don't have a valid range port number.";
			throw std::runtime_error(err);
		}
		itr++;
	}
}

void Parsing::checkDuplicatedPort(std::vector<std::string> ports, std::string port)
{
	if (ports.size() == 0)
		return;
	if (std::find(ports.begin(), ports.end(), port) != ports.end())
		throw std::runtime_error("\x1b[31mError: Duplicated ports between servers.");
}

// NOTE Host checking

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

// NOTE Allowd methods checking

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

// NOTE BodySize checking

void Parsing::checkBodySize(std::vector<std::string> vec, size_t counter)
{
	std::string err;
	std::vector<std::string>::iterator itr;
	if (vec.size() == 0)
		return;
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

// NOTE Location paths checking

void Parsing::checkingForLoactionPath(std::string path, size_t counter, std::string porp) {
	struct stat info;
	std::string err;
	if (stat(path.c_str(), &info) != 0) {
		err = "\x1b[31mError: Server " + intToString(counter) + " have an invalide " + porp + " path. don't exist.";
		std::cout << path << std::endl;
        throw std::runtime_error(err); // Path doesn't exist or err
    }
	if (! (info.st_mode & S_IFDIR)) {
		err = "\x1b[31mError: Server " + intToString(counter) + " have an invalide " + porp + " path. is not a director.";
		throw std::runtime_error(err);
	}
}

// NOTE Root director checking

void Parsing::checkForRootDirectory (std::vector<std::string> path, size_t counter) {
	std::string err;

	if (path.size() == 0)
		return;
	else if (path.size() == 1)
		checkingForLoactionPath(path[0], counter, "root");
	else{
		err = "\x1b[31mError: Server " + intToString(counter) + " have more than one root director.";
		throw std::runtime_error(err);
	}
}


// NOTE error page checking

void Parsing::checkErrorPage(std::vector<std::string> path, size_t counter) {
	std::string err;
	struct stat info;

	if (path.size() == 0) {
		// err = "\x1b[31mError: Server " + intToString(counter) + " no provided path for error page file.";
		// throw std::runtime_error(err);
		return;
	} else 
	if (path.size() > 1) {
		err = "\x1b[31mError: Server " + intToString(counter) + " only one error page file should be provided.";
		throw std::runtime_error(err);

	}
	if (stat(path[0].c_str(), &info) != 0){
		err = "\x1b[31mError: Server " + intToString(counter) + " provided path for error page file don't exist.";
		throw std::runtime_error(err);
	}

	if ((info.st_mode & S_IFDIR)) {
		err = "\x1b[31mError: Server " + intToString(counter) + " provided path for error page file is a director path";
		throw std::runtime_error(err);
	}
}


// NOTE autoIndex check

void Parsing::checkAutoIndex(std::vector<std::string> autoIndex, size_t counter) {
	std::string err;

	if (autoIndex.size() == 0)
		return;
	if (autoIndex.size() > 1) {
		err = "\x1b[31mError: Server " + intToString(counter) + " auto index should only have one value (on, off).";
		throw std::runtime_error(err);
	}
	if (autoIndex[0].compare("on") != 0 && autoIndex[0].compare("off") != 0) {
		err = "\x1b[31mError: Server " + intToString(counter) + " auto index should have an \"on\" or an \"off\" value";
		throw std::runtime_error(err);
	}
}




void Parsing::checkForErrors(std::vector<t_data> data)
{
	std::vector<t_data>::iterator itr;
	std::vector<std::string> portsHolder;
	std::vector<std::string> hostsHolder;
	std::vector<std::string> locationsHolder;
	std::vector<std::string>::iterator locationsHolderItr;
	std::vector<std::string> holder;
	std::vector<std::string>::iterator holderItr;

	checkUnknownKey(data);
	size_t counter = 1;
	itr = data.begin();
	while (itr != data.end())
	{
		holder = getRule(*itr, "listen");		
		checkPortNumber(holder, counter);
		portsHolder.insert(portsHolder.end(), holder.begin(), holder.end());
		if (checkDuplicatedValues(portsHolder))
			throw std::runtime_error("\x1b[31mError: Servers has duplicated ports.");

		holder = getRule(*itr, "host");
		checkHost(getRule(*itr, "host"), counter);
		checkDuplicatedHosts(hostsHolder, getRule(*itr, "host")[0]);
		hostsHolder.push_back(getRule(*itr, "host")[0]);

		holder = getRule(*itr, "allowed_methods");
		checkAllowedMethods(getRule(*itr, "allowed_methods"), counter);
		checkRepeatedMethods(getRule(*itr, "allowed_methods"), counter);

		holder = getRule(*itr, "maxBodySize");
		checkBodySize(holder, counter);

		holder = getRule(*itr, "root");
		checkForRootDirectory(holder, counter);
		
		holder = getRule(*itr, "error_page");
		checkErrorPage(holder, counter);

		holder = getRule(*itr, "autoIndex");
		checkAutoIndex(holder, counter);


		// exit(1);

		locationsHolder = itr->locations;
		locationsHolderItr = locationsHolder.begin();
		while (locationsHolderItr != locationsHolder.end()) {
			std::cout << *locationsHolderItr << std::endl;
			checkingForLoactionPath(*locationsHolderItr, counter, "location");

			holder = getLocationRule(*itr, *locationsHolderItr, "root");
			checkForRootDirectory(holder, counter);
		
			holder = getLocationRule(*itr, *locationsHolderItr, "error_page");
			checkErrorPage(holder, counter);

			holder = getLocationRule(*itr, *locationsHolderItr, "autoIndex");
			checkAutoIndex(holder, counter);


			locationsHolderItr++;
		}
		counter++;
		*itr++;
	}
}
