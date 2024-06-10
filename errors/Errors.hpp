#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include "../parsing/FileReader.hpp"
#include "../parsing/parsingStruct.hpp"
#include "../parsing/Parsing.hpp"


#define RED					"\x1b[31m"
#define GREEN				"\x1b[32m"
#define YELLO				"\x1B[33m"
#define DEFAULT_COLOR		"\x1b[0m"

class ErrorsChecker {
	private:
		std::string		conFilePath;
		std::ifstream	file;
		std::string		conf;

		bool checkExtention (std::string path);
		void checkPortNumber(std::vector<std::string> data, size_t counter);
		void checkDuplicatedPort(Parsing &parse, std::vector<t_data> data);
		void checkHost(std::vector<std::string> data, size_t counter);
		std::vector<std::string> split(std::string str, std::string delimiter);
	public:
		ErrorsChecker(void);
		ErrorsChecker(std::string path);
    	~ErrorsChecker(void);
		void setConfile(std::string path);
		void checkFile(void);
		void checkUnknownKey(std::vector<t_data> data);
		void checkPrimaryValues(Parsing  &parse);


        const std::string GetConfigFilePath() const; // added giter

};


#endif
