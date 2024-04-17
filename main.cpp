/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/17 17:36:33 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"
#include "HttpRequest.hpp"


void StartServer()
{

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
            Server.SendResponse();
            
            std::cout << "full request :"<<std::endl;
            Request.PrintRequest();
            std::cout << "http headers :"<< std::endl;
            Request.PrintHeaders();
            
            std::cout << "http method :"<<std::endl;
            std::cout <<Request.HttpMethod()<<std::endl;
            
            std::cout << "http version"<<std::endl;
            std::cout << Request.HttpVersion()<<::std::endl;
            
            std::cout << "path :"<<std::endl;
            std::cout << Request.Path()<< std::endl;
        }

    } 
    catch (std::runtime_error& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
      return 0;
}

