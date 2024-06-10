#include "FileReader.hpp"


FileReader::FileReader(void){
	this->end = false;
}

FileReader::FileReader(std::string fileName) : fileName(fileName) {
	this->end = false;
	this->file.open(fileName);
	if (!this->file.is_open()) {
		std::cerr << "Error: Couldn't open the requested file, check if file exist"
		<< " and have the permission to open it" << std::endl;
	}
}

void FileReader::readLine(void) {
	if (!std::getline(this->file, this->line)){
		this->line = "";
		this->end = true;
	}
}

std::string FileReader::getLine(void) {
	return (this->line);
}

void FileReader::setLine(std::string newLine){
	this->line = newLine;
}

void FileReader::setFileName(std::string fileName){
	this->fileName = fileName;
}

FileReader::~FileReader(void){
	this->file.close();
}

void FileReader::reset(void){
	this->file.close();
	this->line = "";
	this->end = false;
	this->file.open(this->fileName);
	if (!this->file.is_open())
		throw std::runtime_error("\x1b[31mError: Couldn't open the configuration file.\nCheck if the file exists and have the read permission.");
}

bool FileReader::isEndFile(void){
	return this->end;
}
