/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/07 23:26:43 by ybourais         ###   ########.fr       */
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
            std::cout << Server.GetRequest()<<std::endl;;
            HttpRequest Request(Server.GetRequest());
            Server.SendResponse();

        }

    } 
    catch (std::runtime_error& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
      return 0;
}

