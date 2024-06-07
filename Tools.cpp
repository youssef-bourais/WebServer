

#include "Tools.hpp"

#define FILE_TYPE 0
#define DIR_TYPE 1
#define UNKNOWN_TYPE -1

int checkFileType(const std::string &path) 
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0) 
    {
        if (s.st_mode & S_IFDIR)
            return DIR_TYPE;
        else if (s.st_mode & S_IFREG)
            return FILE_TYPE;
    }
    return UNKNOWN_TYPE;
}
