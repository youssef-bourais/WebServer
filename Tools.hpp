#pragma once

#include <sys/stat.h>
#include <iostream>

#define FILE_TYPE 0
#define DIR_TYPE 1
#define UNKNOWN_TYPE -1


int checkFileType(const std::string &path); 

