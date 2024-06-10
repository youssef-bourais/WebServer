#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <fstream>
#include <iostream>
#include <string>

class FileReader {
	private:
		std::ifstream file;
		std::string line;
		bool end;
		std::string fileName;

	public:
		FileReader(void);
		FileReader(std::string fileName);
		void readLine(void);
		std::string getLine(void);
		~FileReader(void);
		void setLine(std::string newLine);
		void reset(void);
		void setFileName(std::string fileName);
		bool isEndFile(void);
		//void set_file_stream(std::string fileName);
};

#endif