#ifndef PARSING_HPP
#define PARSING_HPP

#include <iostream>
#include "FileReader.hpp"
#include <vector>
#include <ctype.h>
#include <algorithm>
#include "./parsingStruct.hpp"


class Parsing : private FileReader {
	private:
		std::string	filePath;
		std::string	block;
		size_t serversCout;
		std::vector<t_data> data;
		

		Parsing(void);
		bool isAllWhiteSpaces (void) const;
		std::string removeBegeningWhiteSpace(void);
		void collectServerData(void);
		std::string ft_trim(std::string block);
		size_t countAppearance(std::string str, char c);
		bool isWhiteSpace(char c);
		bool ft_strchr(std::string str, char c);
		size_t wordsCounter(std::string str, std::string sp);
		//ft_splite func keeps the delimiter.
		std::vector<std::string> ft_split(std::string str, std::string c);
		void trimVector(std::vector<std::string> &data);
		size_t getOpenCurlyBracePos(std::vector<std::string> data);
		void checkBlock(void);
		std::vector<std::string> initKeys(void);
		void collectDataErrorCheck(void);
		std::vector<std::string> split(std::string str, std::string delimiter);
		std::string getScope(std::string &server);
		std::vector<std::string> removeEmptyPos(std::vector<std::string> vec);
		void checkRepeatedKeywords(std::string str);
		void checkRepeatedLocations(std::vector<std::string> vec);
		void collectLocationsPaths(std::vector<std::string> vec, t_data data, std::vector<std::string> &locations);
		bool isLocationExist(t_data server, std::string loaction);
		std::vector<std::string>::iterator getLocationPosition(t_data &server, std::string location);
		void checkLoactionSemiColon(void);
		void checkRepeatedLocationKeyword(std::vector<std::string> vec);
		std::vector<std::string> collectRulesNames(t_data data);
		void checkUnknownScope(t_data data);
		void checkUnknownKey(std::vector<std::string> rules);
		void checkPortNumber(std::vector<std::string> data, size_t counter);
		void checkDuplicatedPort(std::vector<std::string> ports, std::string port);
		void checkHost(std::vector<std::string> data, size_t counter);
		void checkDuplicatedHosts(std::vector<std::string> data, std::string value);
		void checkAllowedMethods(std::vector<std::string> data, size_t counter);
		void checkRepeatedMethods(std::vector<std::string> data, size_t counter);
		void checkUnknownKey(std::vector<t_data> data);
		void checkBodySize(std::vector<std::string> vec, size_t counter);
		std::string replaceChar(std::string str, std::string toReplace, char with);
		std::string removeChars(std::string str, std::string toRemove);
		int findWordPos(std::string str, std::string word);
		std::string getServer(void);
		

	public:
		std::string getBlockName(void);
		Parsing(std::string filePath);
		~Parsing(void);
		size_t readBlock(void);
		std::string getBlock(void) const;
		bool checkBlockCurlyBraces(void) const;
		size_t countServers(void);
		bool collectData(void);
		void initServerStruct(size_t serversCount);
		std::vector<t_data> getData(void) const;
		std::vector<std::string> getRule(t_data server, std::string ruleName) ;
		std::string getLocationRule(t_data server, std::string location , std::string ruleName);
		void checkForErrors(std::vector<t_data> data);
};

#endif
