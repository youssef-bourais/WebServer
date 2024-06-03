/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/06/03 17:45:02 by ybourais         ###   ########.fr       */
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
            Request.PrintRequest();
            /* PrintRequestInfo(Request); */
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

