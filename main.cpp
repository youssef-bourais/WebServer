/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-bah <sait-bah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/06/30 09:54:29 by sait-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Response/HttpResponse.hpp"
#include "./Server/HttpServer.hpp"
#include "./Request/HttpRequest.hpp"


#include "./errors/Errors.hpp"
#include "./parsing/parse.hpp"

#define DEFAULT_CONF_FILE "./ConfiFile/server.conf"

void PrintConfigFileInfo(const ErrorsChecker &Checker)
{
    Parsing ConfigfFile(Checker.GetConfigFilePath());

    ConfigfFile.readBlock();

    while(ConfigfFile.collectData())
    {
    }

    std::vector<t_data> Data = ConfigfFile.getData();

    int i = 0;
    // while (i < Data.size()) 
    // {
    //     std::cout << ConfigfFile.getLocationRule(Data[i], Data[i].locations[i], "proxy_pass")<<std::endl;
    //     i++;
    //
    // }






    std::vector<t_data>::iterator it = Data.begin();
    while (it != Data.end()) 
    {
        std::cout <<"++++++++++Server+++++++++++"<<std::endl;

        // Locations
        std::vector<std::string>::iterator Locations = it->locations.begin();
        std::cout <<"======Locations======"<<std::endl;
        while(Locations != it->locations.end())
        {
            std::cout << *Locations<<std::endl;
            Locations++;
        }
 
        // Scopes
        std::vector<std::string>::iterator Scopes =  it->scopes.begin();
        std::cout <<"======Scopes======"<<std::endl;
        while(Scopes != it->scopes.end())
        {

            std::cout << *Scopes<<std::endl;
            Scopes++;
        }

        //Ruels (Key Value)
        std::vector<std::string>::iterator RulesNames = it->rulesNames.begin();
        std::cout <<std::endl<<"======RulesNames======"<<std::endl;
        while(RulesNames != it->rulesNames.end())
        {
            std::vector<std::string> Value = ConfigfFile.getRule(*it, *RulesNames);
            if (Value.size() != 0)
                std::cout << *RulesNames<<std::endl;
                // std::cout << *RulesNames<< " : don't have value\n";
            // else 
            // {
            //     std::cout << *RulesNames<< " [" << Value.size()<< "] : ";
            //     std::vector<std::string>::iterator itValue = Value.begin();
            //     while(itValue != Value.end())
            //     {
            //         std::cout << *itValue << ", ";
            //         itValue++;
            //     }
            //     std::cout << std::endl;
            // }
            RulesNames++;
        }

        // std::string loc = ConfigfFile.getLocationRule(Data[0], Data[0].locations[0], "proxy_pass");
        // std::cout << loc << std::endl;
        // std::cout << Data[0].locations[0] << std::endl;

        // exit(0);
        // scope name;
        std::cout <<"======ScopeName======"<<std::endl;
        std::cout << it->scopName<<std::endl;

        // scope;
        std::cout <<"======Scope======"<<std::endl;
        std::cout << it->scope<<std::endl;

        it++;
    }

    // std::vector<t_data>::iterator it = servers.begin();
    // while(it != servers.end())
    // {
    //
    //     it++;
    // }

    //check for error;
    // ConfigfFile.checkForErrors(Data);
}

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
        Parsing parse(DEFAULT_CONF_FILE);

        std::cout << "===============START==============="<<std::endl;
        std::vector<t_servers> servers = parse.getServers();
        
        std::cout << servers[0].locations[0].root << std::endl;
        return (0);
        // PrintConfigFileInfo(checker);

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
            std::cout << Request.GetHost()<<std::endl;
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

