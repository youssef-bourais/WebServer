#include "Parsing.hpp"

Parsing::Parsing(void)
{
}

Parsing::Parsing(std::string filePath) : FileReader(filePath)
{
	std::vector<t_data> data;
	readBlock();
	std::cout << "Parsing start" << std::endl;
	while (collectData())
	{
	}
	data = getData();
	checkForErrors(data);
}

Parsing::~Parsing(void)
{
}

size_t Parsing::readBlock(void)
{
	size_t counter = 0, lastNewLinePos,
		   lineCounter = 0, openBraceCounter = 0,
		   closeBraceCounter = 0;
	std::vector<std::string> holder;

	this->block = "";
	while (!this->isEndFile())
	{
		readLine();
		lineCounter++;
		counter += this->getLine().length();
		this->block.append(this->getLine());
		this->block.append("\n");
		if (this->getLine().find("{") != std::string::npos)
			openBraceCounter += countAppearance(this->getLine(), '{');
		if (this->getLine().find("}") != std::string::npos)
		{
			closeBraceCounter += countAppearance(this->getLine(), '}');
		}
	}
	if (this->isEndFile() && openBraceCounter != closeBraceCounter)
	{

		std::string err = "\x1b[31mError: curly braces don't match";
		throw std::runtime_error(err.c_str());
	}
	this->block = ft_trim(this->block);
	return counter;
}

std::string Parsing::getBlock(void) const
{
	return this->block;
}

void Parsing::collectDataErrorCheck(void)
{
	size_t curlyBracePos, curlyBrace;

	curlyBracePos = this->block.find("{");
	if (curlyBracePos == std::string::npos)
		throw std::runtime_error("\x1b[31mError: No scope has been found.");
	if (curlyBracePos == 0)
		throw std::runtime_error("\x1b[31mError: Server block has not been found.");

	curlyBracePos = this->block.find("}");
	if (curlyBracePos == std::string::npos)
		throw std::runtime_error("\x1b[31mError: Unclosed curly brace.");
	curlyBracePos = countAppearance(this->block, '{');
	curlyBrace = countAppearance(this->block, '}');
	if (curlyBracePos != curlyBrace)
		throw std::runtime_error("\x1b[31mError: Unclosed curly brace.");
}

std::string Parsing::getScope(std::string &server)
{
	size_t idx = 0, idx2 = 0,
		   openCounter = 0, closeCounter = 0;

	idx = server.find("location");
	if (idx == std::string::npos)
		return "";
	while (server[idx + idx2])
	{
		if (server[idx + idx2] == '{')
			openCounter++;
		if (server[idx + idx2] == '}')
			closeCounter++;
		if (openCounter == closeCounter && openCounter > 0)
			break;
		idx2++;
	}

	std::string holder = server.substr(idx, idx2 + 1);
	server = server.erase(idx, idx2 + 1);
	return holder;
}

void Parsing::checkRepeatedKeywords(std::string str)
{
	std::vector<std::string> vec, keys;
	std::vector<std::string>::iterator itr;
	std::string holder, err;

	vec = split(str, ";");
	vec = removeEmptyPos(vec);
	itr = vec.begin();
	trimVector(vec);
	while (itr != vec.end())
	{
		holder = itr->substr(0, itr->find_first_of(" \t\n"));
		if (std::find(keys.begin(), keys.end(), holder) != keys.end())
		{
			err = "\x1b[31mError: Duplicated keyword " + holder + ".";
			throw std::runtime_error(err);
		}
		keys.push_back(holder);
		*itr++;
	}
}

void Parsing::checkRepeatedLocations(std::vector<std::string> vec)
{
	std::vector<std::string>::iterator itr;
	std::vector<std::string> locations;

	itr = vec.begin();
	trimVector(vec);
	while (itr != vec.end())
	{

		*itr = itr->erase(0, itr->find_first_of(" \t\n"));
		*itr = ft_trim(*itr);
		if (std::find(locations.begin(), locations.end(), itr->substr(0, itr->find_first_of(" \t\n"))) != locations.end())
			throw std::runtime_error("\x1b[31mError: Duplicated location.");
		locations.push_back(itr->substr(0, itr->find_first_of(" \t\n")));
		*itr++;
	}
}

void Parsing::collectLocationsPaths(std::vector<std::string> vec, t_data data, std::vector<std::string> &locations)
{
	std::vector<std::string>::iterator itr;

	itr = vec.begin();
	while (itr != vec.end())
	{
		*itr = ft_trim(*itr);
		*itr = itr->erase(0, itr->find_first_of(" \t\n"));
		*itr = ft_trim(*itr);
		if (itr->substr(0, itr->find_first_of(" \t\n{")).find_first_not_of(" \t\n{") == std::string::npos)
			throw std::runtime_error("\x1b[31mError: Location don't have a path.");
		locations.push_back(itr->substr(0, itr->find_first_of(" \t\n{")));

		// std::cout << *itr << std::endl;
		*itr++;
	}
}

void Parsing::checkLoactionSemiColon(void)
{
	size_t idx = 0;

	idx = this->block.find("location");
	if (idx == 0 || idx == std::string::npos)
		return;
	idx--;
	while (idx > 0)
	{
		if (this->block[idx] != ' ' && this->block[idx] != '\t' && this->block[idx] != '\n' && this->block[idx] != ';')
		{

			throw std::runtime_error("\x1b[31mError: Missing semi colon");
		}
		else if (this->block[idx] == ';')
			break;
		idx--;
	}
}

void Parsing::checkRepeatedLocationKeyword(std::vector<std::string> vec)
{
	std::vector<std::string>::iterator itr, holderItr;
	std::vector<std::string> collectedKeys, holder;
	size_t idx;

	itr = vec.begin();
	while (itr != vec.end())
	{
		idx = itr->find_first_of("{");
		*itr = (idx == std::string::npos ? *itr : itr->erase(0, idx + 1));
		idx = itr->rfind("}");
		*itr = (idx == std::string::npos ? *itr : itr->erase(idx, itr->length()));
		holder = split(*itr, ";");
		trimVector(holder);
		holder = removeEmptyPos(holder);

		holderItr = holder.begin();
		while (holderItr != holder.end())
		{
			if (std::find(collectedKeys.begin(), collectedKeys.end(), holderItr->substr(0, holderItr->find_first_of(" \t\n"))) != collectedKeys.end())
			{
				std::cout << *holderItr << std::endl;

				throw std::runtime_error("\x1b[31mError: Location rule duplicated");
			}
			collectedKeys.push_back(holderItr->substr(0, holderItr->find_first_of(" \t\n")));
			*holderItr++;
		}
		collectedKeys.clear();
		*itr++;
	}
}

void Parsing::checkUnknownScope(t_data data)
{
	if (data.scope.find_first_of("{") != std::string::npos)
		throw std::runtime_error("\x1b[31mError: Unknown scope");
}

std::vector<std::string> Parsing::collectRulesNames(t_data data)
{
	std::vector<std::string>::iterator itr, holderItr;
	std::vector<std::string> holder, names;

	holder = split(data.scope, ";");
	trimVector(holder);
	itr = holder.begin();
	while (itr != holder.end())
	{
		names.push_back(itr->substr(0, itr->find_first_of(" \t\n")));
		*itr++;
	}
	itr = data.scopes.begin();
	while (itr != data.scopes.end())
	{
		*itr = itr->erase(0, itr->find_first_of("{") + 1);
		*itr = itr->erase(itr->find_first_of("}"));
		*itr = ft_trim(*itr);
		holder = split(*itr, ";");
		trimVector(holder);
		holderItr = holder.begin();
		while (holderItr != holder.end())
		{
			*holderItr = ft_trim(*holderItr);
			names.push_back(holderItr->substr(0, holderItr->find_first_of(" \t\n")));
			holderItr++;
		}
		*itr++;
	}
	names = removeEmptyPos(names);
	return names;
}

void Parsing::checkUnknownKey(std::vector<std::string> rules)
{
}

std::string Parsing::getServer(void)
{
	std::string server;
	size_t idx;
	std::string holder;
	std::string err;
	size_t openCurlyCount = 0;
	size_t closeCurlyCount = 0;

	idx = 0;
	idx = this->block.find_first_of("{");
	holder = this->block.substr(0, idx);
	holder = ft_trim(holder);

	if (holder.compare("server") != 0)
	{
		err = "\x1b[31mError: Unknown scope " + holder + ".";
		throw std::runtime_error(err);
	}

	idx = 0;
	while (this->block[idx])
	{
		if (this->block[idx] == '{')
			openCurlyCount++;
		else if (this->block[idx] == '}')
			closeCurlyCount++;
		else if (openCurlyCount == closeCurlyCount && openCurlyCount > 1)
		{
			break;
		}
		idx++;
	}
	holder = this->block.substr(0, idx);

	this->block = this->block.erase(0, idx);
	this->block = ft_trim(this->block);
	return holder;
}

bool Parsing::collectData(void)
{
	size_t idx;
	std::string holder, server;
	t_data dataHolder;
	std::vector<std::string> scopes, rules;
	std::vector<std::string>::iterator itr;

	if (this->block.length() == 0)
		return false;

	collectDataErrorCheck();
	server = getServer();

	// just removing scop name.
	idx = server.find_first_of("{");
	holder = server.substr(0, idx);
	holder = ft_trim(holder);
	if (holder.compare("server") != 0)
		throw std::runtime_error("\x1b[31mError: Unknown scope.");
	server = server.erase(0, idx + 1);
	server = ft_trim(server);
	idx = server.find_last_of("}");
	server = server.erase(idx, server.length());
	server = ft_trim(server);
	// 00000000000000000000000000000000000000000000000000
	//  std::cout << "00000000000000000000000000000000000000" << std::endl;

	while (server.find("location") != std::string::npos)
	{
		checkLoactionSemiColon();
		scopes.push_back(getScope(server));
	}
	trimVector(scopes);
	dataHolder.scope = server;
	dataHolder.scopes = scopes;

	checkRepeatedKeywords(dataHolder.scope);
	checkRepeatedLocations(dataHolder.scopes);
	collectLocationsPaths(dataHolder.scopes, dataHolder, dataHolder.locations);
	checkRepeatedLocationKeyword(dataHolder.scopes);
	checkUnknownScope(dataHolder);
	dataHolder.rulesNames = collectRulesNames(dataHolder);

	this->data.push_back(dataHolder);
	std::cout << "\x1b[32m+ Collecting data" << std::endl;
	return true;
}

std::vector<t_data> Parsing::getData(void) const
{
	return this->data;
}

std::vector<std::string> Parsing::removeEmptyPos(std::vector<std::string> vec)
{
	std::vector<std::string>::iterator itr;
	std::vector<std::string> data;
	size_t idx;

	itr = vec.begin();
	while (itr != vec.end())
	{
		idx = itr->find_first_not_of(" \t\n");
		if (idx != std::string::npos && itr->length() > 0)
		{
			data.push_back(*itr);
		}
		*itr++;
	}
	return data;
}

std::vector<std::string> Parsing::getRule(t_data server, std::string ruleName)
{
	std::vector<std::string> rules, selectedRule, fallback;
	std::vector<std::string>::iterator itr;

	rules = split(server.scope, ";");
	trimVector(rules);
	itr = rules.begin();
	while (itr != rules.end())
	{
		selectedRule = split(*itr, " \t\n");
		if (selectedRule.size() > 0 && selectedRule[0].compare(ruleName) == 0)
		{
			selectedRule.erase(selectedRule.begin());
			return removeEmptyPos(selectedRule);
		}
		*itr++;
	}
	return fallback;
}

bool Parsing::isLocationExist(t_data server, std::string location)
{
	std::vector<std::string>::iterator itr;

	itr = std::find(server.locations.begin(), server.locations.end(), location);
	if (itr == server.locations.end())
		return false;
	return true;
}

std::vector<std::string>::iterator Parsing::getLocationPosition(t_data &server, std::string location)
{
	std::vector<std::string>::iterator itr;

	itr = server.scopes.begin();
	while (itr != server.scopes.end())
	{
		*itr = ft_trim(*itr);
		*itr = itr->erase(0, itr->find_first_of(" \t\n"));
		*itr = ft_trim(*itr);
		if (location.compare(itr->substr(0, itr->find_first_of(" \t\n{"))) == 0)
		{
			return itr;
		}
		*itr++;
	}
	return itr;
}

std::string Parsing::getLocationRule(t_data server, std::string location, std::string ruleName)
{
	std::vector<std::string>::iterator itr;
	std::vector<std::string> data;
	std::string holder;

	if (!isLocationExist(server, location))
	{
		return "";
	}
	itr = getLocationPosition(server, location);
	*itr = itr->erase(0, itr->find_first_of("{") + 1);
	*itr = ft_trim(*itr);
	data = split(*itr, ";");
	trimVector(data);
	itr = data.begin();
	while (itr != data.end())
	{
		if (ft_trim(itr->substr(0, itr->find_first_of(" \t\n"))).compare(ruleName) == 0)
		{
			*itr = itr->erase(0, itr->find_first_of(" \t\n"));
			*itr = ft_trim(*itr);
			return itr->substr(0, itr->find_first_of(" \t\n"));
		}
		*itr++;
	}
	return "";
}
