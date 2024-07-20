#pragma once

#define FILE_TYPE 0
#define DIR_TYPE 1
#define UNKNOWN_TYPE -1

#include "../parsing/parsingStruct.hpp"
#include "../parsing/Parsing.hpp"
#include "../Response/HttpResponse.hpp"


#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

#include <ctime> // Correct include for time functions
#include <sstream> // For stringstream to replace std::to_string
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <cstdlib>
#include <cstring>
#include <dirent.h>





#define FILE_TYPE 0
#define DIR_TYPE 1
#define UNKNOWN_TYPE -1



void deleteDir(const std::string& path);
bool isDirectory(const std::string& path);
std::string removeLast(std::string str);
std::string Delete (RequestParsser &Request, HttpResponse &Response, t_servers &setting, int index, std::string Uri);
int checkFileType(const std::string &path); 
void ImOut(std::string msg);
void printConfigFile(std::string Path);
void PrintRequestInfo(const RequestParsser &Request);
void printVect(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::string Key);
std::string intToString(int value) ;
int StringToInt(const std::string &s);
void SetSocketToNonBlocking(int fd);
std::string HtmlToSring(std::string filePath);

std::string GetHttpStatusMessage(HTTPStatusCode StatusCode);

std::string ModifyHtmlPage(std::string &htmlPage, HTTPStatusCode statusCode, std::string Path); 
std::string err_pages(t_servers ServerSetting, int index, HTTPStatusCode StatusCode);
