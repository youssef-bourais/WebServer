#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include "./Request/RequestParsser.hpp"
#include "./Response/HttpResponse.hpp"



void deleteDir(const std::string& path) 
{
    DIR* dir = opendir(path.c_str());

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
        {
            continue;
        }
         std::string fullPath = path + "/" + entry->d_name;
        if (entry->d_type == DT_DIR) 
        {
            // Recursively delete subdirectories
            deleteDir(fullPath);
            std::remove(fullPath.c_str()); // Remove the directory after contents are removed
        } 
        else 
        {
            // Remove the file
            if (std::remove(fullPath.c_str()) != 0) 
            {
                std::cerr << "Error removing file: " << fullPath << std::endl;
            }
        }
    }

    closedir(dir);
}

bool isDirectory(const std::string& path)
{
    struct stat fileStat; // get the file status
    if (stat(path.c_str(), &fileStat) == 0) // check if the file exists
    {
        return S_ISDIR(fileStat.st_mode); // check if it is a directory
    }
    else
    {
        std::cerr << "Error: " << std::strerror(errno) << std::endl; // error
        return false;
    }
}

std::string removeLast(std::string str) 
{
    if (!str.empty()) 
    {
        str.resize(str.size() - 1); // Resize to remove the last character
    }
    return str; // Return the modified string
}


void Delete (RequestParsser Request, HttpResponse &Response)
{
    std::string requestPath = Request.GetPath();
    std::string rootPath = "./" + requestPath;  
    if (access(rootPath.c_str(), F_OK) == -1) // check if file exists
    {
        Response.SetHTTPStatusCode(HTTP_NOT_FOUND);
        return;
    }
    if (isDirectory(rootPath)) 
    {
        if (!requestPath.empty() && requestPath.back() == '/') // check if the path ends with '/'
        {
            if (access(rootPath.c_str(), W_OK) == 0) // check permission
            {
                deleteDir(removeLast(rootPath)); // delete the file
                std::remove(rootPath.c_str());
            }
            else 
                Response.SetHTTPStatusCode(HTTP_FORBIDDEN);
        }
        else 
            Response.SetHTTPStatusCode(HTTP_CONFLICT);
    }
    else 
    {
        if (std::remove(rootPath.c_str()) != 0) // remove the file
            Response.SetHTTPStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        else
            Response.SetHTTPStatusCode(HTTP_NO_CONTENT);
    }
}

