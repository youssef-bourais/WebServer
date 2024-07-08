#pragma once

#include "../parsing/parsingStruct.hpp"
#include "../parsing/Parsing.hpp"
#include "../Request/HttpRequest.hpp"

#define FILE_TYPE 0
#define DIR_TYPE 1
#define UNKNOWN_TYPE -1
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

#include <ctime> // Correct include for time functions
#include <sstream> // For stringstream to replace std::to_string



#define FILE_TYPE 0
#define DIR_TYPE 1
#define UNKNOWN_TYPE -1


int checkFileType(const std::string &path); 
void ImOut(std::string msg);
void printConfigFile(std::string Path);
void PrintRequestInfo(const HttpRequest &Request);
void printVect(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::string Key);
std::string intToString(int value) ;
int StringToInt(const std::string &s);
void SetSocketToNonBlocking(int fd);
