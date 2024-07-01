/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/01 17:53:41 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Response/HttpResponse.hpp"
#include "./Server/HttpServer.hpp"
#include "./Request/HttpRequest.hpp"


#include "./errors/Errors.hpp"
#include "./parsing/parse.hpp"
#include <cstdint>
#include <sys/resource.h>

// void PrintConfigFileInfo(const ErrorsChecker &Checker)
// {
//     Parsing ConfigfFile(Checker.GetConfigFilePath());
//
//     ConfigfFile.readBlock();
//
//     while(ConfigfFile.collectData())
//     {
//     }
//
//     std::vector<t_data> Data = ConfigfFile.getData();
//
//     int i = 0;
//     // while (i < Data.size()) 
//     // {
//     //     std::cout << ConfigfFile.getLocationRule(Data[i], Data[i].locations[i], "proxy_pass")<<std::endl;
//     //     i++;
//     //
//     // }
//
//     std::vector<t_data>::iterator it = Data.begin();
//     while (it != Data.end()) 
//     {
//         std::cout <<"++++++++++Server+++++++++++"<<std::endl;
//
//         // Locations
//         std::vector<std::string>::iterator Locations = it->locations.begin();
//         std::cout <<"======Locations======"<<std::endl;
//         while(Locations != it->locations.end())
//         {
//             std::cout << *Locations<<std::endl;
//             Locations++;
//         }
//
//         // Scopes
//         std::vector<std::string>::iterator Scopes =  it->scopes.begin();
//         std::cout <<"======Scopes======"<<std::endl;
//         while(Scopes != it->scopes.end())
//         {
//
//             std::cout << *Scopes<<std::endl;
//             Scopes++;
//         }
//
//         //Ruels (Key Value)
//         std::vector<std::string>::iterator RulesNames = it->rulesNames.begin();
//         std::cout <<std::endl<<"======RulesNames======"<<std::endl;
//         while(RulesNames != it->rulesNames.end())
//         {
//             std::vector<std::string> Value = ConfigfFile.getRule(*it, *RulesNames);
//             if (Value.size() != 0)
//                 std::cout << *RulesNames<<std::endl;
//                 // std::cout << *RulesNames<< " : don't have value\n";
//             // else 
//             // {
//             //     std::cout << *RulesNames<< " [" << Value.size()<< "] : ";
//             //     std::vector<std::string>::iterator itValue = Value.begin();
//             //     while(itValue != Value.end())
//             //     {
//             //         std::cout << *itValue << ", ";
//             //         itValue++;
//             //     }
//             //     std::cout << std::endl;
//             // }
//             RulesNames++;
//         }
//
//         // std::string loc = ConfigfFile.getLocationRule(Data[0], Data[0].locations[0], "proxy_pass");
//         // std::cout << loc << std::endl;
//         // std::cout << Data[0].locations[0] << std::endl;
//
//         // exit(0);
//         // scope name;
//         std::cout <<"======ScopeName======"<<std::endl;
//         std::cout << it->scopName<<std::endl;
//
//         // scope;
//         std::cout <<"======Scope======"<<std::endl;
//         std::cout << it->scope<<std::endl;
//
//         it++;
//     }
//
//     // std::vector<t_data>::iterator it = servers.begin();
//     // while(it != servers.end())
//     // {
//     //
//     //     it++;
//     // }
//
//     //check for error;
//     // ConfigfFile.checkForErrors(Data);
// }

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

void printVect(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, std::string Key)
{
    std::cout << Key<<": ";
    while(begin != end)
    {
        std::cout << *begin<<", ";
        begin ++;
    }
    std::cout <<std::endl;
}

void printConfigFile(std::string Path)
{
    Parsing parse(Path);
    std::vector<t_servers> ServerSetting = parse.getServers();
    
    int i = 0;
    while(i < ServerSetting.size())
    {
        std::cout << "Server++++++"<<std::endl;
        printVect(ServerSetting[i].listen.begin(), ServerSetting[i].listen.end(), "listen");
        std::cout << "Host: " + ServerSetting[i].host<<std::endl;
        printVect(ServerSetting[i].server_names.begin(), ServerSetting[i].server_names.end(), "server_names");
        std::cout << "maxBodySize: " + ServerSetting[i].maxBodySize<<std::endl;
        printVect(ServerSetting[i].allowedMethods.begin(), ServerSetting[i].allowedMethods.end(), "allowedMethods");
        printVect(ServerSetting[i].index.begin(), ServerSetting[i].index.end(), "index");
        std::cout << "error_pages: " + ServerSetting[i].errPage<<std::endl;
        std::cout << "autoIndex: " << ServerSetting[i].autoIndex<<std::endl;
        std::cout << "root: " + ServerSetting[i].root<<std::endl;

        int j = 0;
        while(j < ServerSetting[i].locations.size())
        {
            std::cout << "Locations++++++"<< std::endl;

            std::cout << "location: " + ServerSetting[i].locations[j].location<<std::endl;
            std::cout << "root: "<<ServerSetting[i].locations[j].root << std::endl;
            std::cout << "autoindex: "<<ServerSetting[i].locations[j].autoIndex << std::endl;
            printVect(ServerSetting[i].locations[j].index.begin(), ServerSetting[i].locations[j].index.end(), "index");
            std::cout << "maxBodySize: "<<ServerSetting[i].locations[j].maxBodySize << std::endl;
            std::cout << "errPage: "<<ServerSetting[i].locations[j].errPage << std::endl;
            printVect(ServerSetting[i].locations[j].allowedMethods.begin(), ServerSetting[i].locations[j].allowedMethods.end(), "allowedMethods");
            
            std::cout << "proxy_pass: "<<ServerSetting[i].locations[j].proxyPass<<std::endl;
            std::cout << "cgiExtentions: "<<ServerSetting[i].locations[j].cgiExtentions<<std::endl;
            std::cout << "cgiPath: "<<ServerSetting[i].locations[j].cgiPath<<std::endl;
            std::cout << "uploadPath: "<<ServerSetting[i].locations[j].uploadPath<<std::endl;
            j++;
        }
        i++;
    }
}

int main(int ac, char **av) 
{
    ErrorsChecker checker;
    if (ac == 2)
		checker.setConfile(av[1]);
    else if (ac == 1) // NOLINT 
		checker.setConfile(DEFAULT_CONF_FILE);
    else // NOLINT
    {
		std::cerr << RED << "Error: webserv can take a config file path or nothing to use the default config file." << std::endl;
		return 1;
	}
    try 
    {
		checker.checkFile();

        printConfigFile(checker.GetConfigFilePath());
        std::cout << "===============START==============="<<std::endl;
        
        HttpServer Server;
        Server.ForceReuse();
        Server.BindSocketToAddr();
        Server.StartListining(3);
        while(1)
        {
            Server.AccepteConnectionAndRecive();
            HttpRequest Request(Server.GetRequest());
        
            PrintRequestInfo(Request);
            HttpResponse Response(Request);
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

