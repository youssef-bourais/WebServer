/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/06/09 17:38:30 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "HttpServer.hpp"
#include "HttpRequest.hpp"


void PrintRequestInfo(const HttpRequest &Request)
{
    std::cout << std::endl<<"++++++++++++++++++| Http  Method |++++++++++++++++"<<std::endl;
    std::cout<< Request.GetHttpMethod()<<std::endl;
    
    std::cout << std::endl<<"++++++++++++++++++|  Http   URI  |++++++++++++++++"<<std::endl;
    std::cout << Request.GetPath()<<std::endl;
    
    std::cout << std::endl<<"++++++++++++++++++| Http Version |++++++++++++++++"<<std::endl;
    std::cout << Request.GetHttpVersion()<<std::endl;
    std::cout << std::endl<<"++++++++++++++++++| Http Headers |++++++++++++++++"<<std::endl;
    Request.PrintHeaders();
    std::cout << std::endl<<"++++++++++++++++++|  Http Request Body  |++++++++++++++++"<<std::endl;
    if(Request.GetBody().empty())
        std::cout << "No Body in Request :("<<std::endl;
    else
        std::cout << Request.GetBody()<<std::endl;
}

int main() 
{
    try 
    {
        HttpServer Server;
        Server.ForceReuse();
        Server.BindSocketToAddr();
        Server.StartListining(3);
        while(1)
        {
            Server.AccepteConnectionAndRecive();
            HttpRequest Request(Server.GetRequest());
            // Request.PrintRequest();
            // std::cout << "========================"<<std::endl;
            PrintRequestInfo(Request);
            HttpResponse Response(Request);
            /* std::cout <<Response.GetResponseBody()<<std::endl; */
            Server.SendResponse(Response);
        }
    } 
    catch (std::runtime_error& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

#include <sys/stat.h>
#include <string>
#include <dirent.h>
#include <iostream>

#define FILE_TYPE 0
#define DIR_TYPE 1
#define UNKNOWN_TYPE -1

// int checkFileType(const std::string &path) 
// {
//     struct stat s;
//     if (stat(path.c_str(), &s) == 0) 
//     {
//         if (s.st_mode & S_IFDIR)
//             return DIR_TYPE;
//         else if (s.st_mode & S_IFREG)
//             return FILE_TYPE;
//     }
//     return UNKNOWN_TYPE;
// }

// void listDirectoryContents(const std::string &dirPath) 
// {
//     DIR *dir;
//     struct dirent *entry;
//
//     if ((dir = opendir(dirPath.c_str())) != NULL) 
//     {
//         while ((entry = readdir(dir)) != NULL) 
//         {
//             std::string entryName = entry->d_name;
//             if (entryName == "." || entryName == "..") 
//                 continue;
//             std::string fullPath = dirPath + "/" + entryName;
//             int type = checkFileType(fullPath);
//
//             if (type == DIR_TYPE) 
//                 std::printf("Dir: %s\n", entryName.c_str());
//             else if (type == FILE_TYPE) 
//                 std::printf("File: %s\n", entryName.c_str());
//             else
//                 std::printf("Unknown: %s\n", entryName.c_str());
//         }
//         closedir(dir);
//     }
//     else 
//         perror("Unable to open directory");
// }

// int main() {
//     std::string dirPath = "minipage/";
//     listDirectoryContents(dirPath);
//     return 0;
// }
