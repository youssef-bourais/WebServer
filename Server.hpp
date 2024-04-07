/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 22:16:30 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/07 20:32:49 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <netinet/in.h> // struct sockaddr_in
#include <sys/errno.h>
#include <sys/socket.h>
#include <iostream>
 #include <unistd.h>
#include "header.hpp"

#define SA struct sockaddr_in
#define PORT 80

class Server
{
    private:
        int ServerFd;
        int FdConnection;
        SA Address;
        char RecivedRequest[MAXLEN + 1];
    public:
        Server();
        ~Server();
        Server &operator=(Server const &s);
        Server(Server const &src);


        void ForceReuse() const;
        void BindSocketToAddr() const;
        void StartListining(int PendingConection) const;
        void AccepteConnectionAndRecive();
        void SendResponse() const;
};

Server::Server()
{
    this->ServerFd = socket(PF_INET, SOCK_STREAM, 0);
    if (ServerFd  == -1) 
    { 
        throw std::runtime_error(std::string("Socket: ") + strerror(errno));
    }
    memset(&Address, 0,sizeof(Address));
    Address.sin_family = PF_INET;
    Address.sin_addr.s_addr = htonl(INADDR_ANY); // we use htonl to convert the INADDR_ANY constant to network byte order
    Address.sin_port = htons(PORT);
    memset(RecivedRequest, 0, MAXLEN);
}

Server::~Server()
{
    close(ServerFd);
}

void Server::ForceReuse() const
{
    int opt = 1;
    // it helps in reuse of address and port. Prevents error such as: “address already in use”
    if (setsockopt(this->ServerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        throw std::runtime_error(std::string("setsockopt:") + strerror(errno));
    }
}

void Server::BindSocketToAddr() const
{

    if(bind(ServerFd, (struct sockaddr *)&Address, sizeof(Address)) < 0)
    {
        throw std::runtime_error(std::string("bind: ") + strerror(errno));
    }

}

void Server::StartListining(int PendingConection) const
{
    if(listen(ServerFd, PendingConection) < 0)
    {
        throw std::runtime_error(std::string("listen:") + strerror(errno));
    }
}

void Server::AccepteConnectionAndRecive()
{
    std::cout << "waiting for conection on Port: " << PORT <<std::endl;
    // extracts the first connection request on the queue of pending connections for the listening socket
    this->FdConnection = accept(ServerFd, (struct sockaddr *)NULL, NULL);
    if(FdConnection < 0)
    {
        throw std::runtime_error(std::string("accept:") + strerror(errno));
    }
    int r = read(FdConnection, RecivedRequest, MAXLEN - 1);
    if(r < 0)
    {
        throw std::runtime_error(std::string("reading from file:"));
    }
}

void Server::SendResponse() const
{
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "\r\n";
    response += "<html>\n";
    response += "<body>\n";
    response += "Hi Mom\n";
    response += "</body>\n";
    response += "</html>\n";
    
    int response_length = strlen(response.c_str());
    if(write(FdConnection, response.c_str(), response_length) != response_length) 
    {
        throw std::runtime_error(std::string("write to FdConnection:"));
    }
    close(FdConnection);
}


