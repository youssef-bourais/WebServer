/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/07 20:32:44 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <netinet/in.h> // struct sockaddr_in
#include <string>
#include <sys/socket.h> // socket, bind, listen
#include <unistd.h>
#include <string.h>

#include <sstream>
#include "header.hpp"
#include "Server.hpp"

#define SA struct sockaddr_in
#define PORT 80
/* #define MAXLEN 4096 */
#define GET "GET"
#define OPTION "OPTION"

void StartServer()
{

}

int main() 
{
    
    try 
    {
        Server Server;
        Server.ForceReuse();
        Server.BindSocketToAddr();
        Server.StartListining(3);
        while(1)
        {
            Server.AccepteConnectionAndRecive();
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

        /* if (send(fdconnection, response, response_length, 0) != response_length)  */
        /* { */
        /*     std::cerr << "Send error: " << strerror(errno) << std::endl; */
        /*     close(fdconnection); */
        /*     return 1; */
        /* } */
