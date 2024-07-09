/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/09 16:15:11 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Response/HttpResponse.hpp"
#include "./Server/HttpServer.hpp"
#include "./Request/HttpRequest.hpp"


#include "./errors/Errors.hpp"
#include "./parsing/parse.hpp"
#include <sys/resource.h>
#include "Tools/Tools.hpp"



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

        // printConfigFile(checker.GetConfigFilePath());
        std::cout << "===============START==============="<<std::endl;
        
        HttpServer Server(checker);
        std::cout <<"waiting for conection on Port: " << PORT <<std::endl;
        Server.AccepteMultipleConnectionAndRecive();
    } 
    catch (std::runtime_error& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

