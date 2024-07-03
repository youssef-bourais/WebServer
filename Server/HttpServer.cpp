/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 21:14:22 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/03 03:20:56 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"
#include <cstdlib>
#include <stdint.h> // Include for uint32_t definition
#include <sys/socket.h>
#include "../Tools/Tools.hpp"


HttpServer::HttpServer(const ErrorsChecker &checker)
{
    // PF_INET: ipv4 or (AF_UNSPEC) for both
    // SOCK_STREAM: TCP
    this->ServerFd = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerFd  == -1) 
    { 
        throw std::runtime_error(std::string("Socket: ") + strerror(errno));
    }
    memset(&Address, 0,sizeof(Address));
    Address.sin_family = AF_INET; // ipv4
    Address.sin_addr.s_addr = htonl(INADDR_ANY); // any availabe local iP, we use htonl to convert the INADDR_ANY constant to network byte order
    Address.sin_port = htons(PORT); // 
    memset(RecivedRequest, 0, MAXLEN);
    
    Parsing parse(checker.GetConfigFilePath());
    this->ServerSetting = parse.getServers();
}

HttpServer::~HttpServer()
{
    close(ServerFd);
}

std::string HttpServer::GetRequest() const
{
    return this->RecivedRequest;
}

void HttpServer::ForceReuse() const
{
    int opt = 1;
    // it helps in reuse of address and port. Prevents error such as: “address already in use”
    if (setsockopt(this->ServerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        throw std::runtime_error(std::string("setsockopt:") + strerror(errno));
    }
}

void HttpServer::BindSocketToAddr() const
{

    if(bind(ServerFd, (struct sockaddr *)&Address, sizeof(Address)) < 0)
    {
        throw std::runtime_error(std::string("bind: ") + strerror(errno));
    }
}

void HttpServer::StartListining(int PendingConection) const
{
    //socket turn into passive listening state
    if(listen(ServerFd, PendingConection) < 0)
    {
        throw std::runtime_error(std::string("listen:") + strerror(errno));
    }
}


#include <iostream>
#include <arpa/inet.h>     // for inet_ntop

void printClientInfo(const struct sockaddr* client_addr, socklen_t client_addr_size) 
{
    if (client_addr == NULL || client_addr_size == 0) 
    {
        std::cerr << "Invalid client address information." << std::endl;
        return;
    }

    // Check address family (IPv4 or IPv6)
    if (client_addr->sa_family == AF_INET) 
    {
        const struct sockaddr_in* addr_in = reinterpret_cast<const struct sockaddr_in*>(client_addr);

        // Convert IP address to string
        char ip_str[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &(addr_in->sin_addr), ip_str, sizeof(ip_str)) == NULL) 
        {
            perror("inet_ntop");
            return;
        }
        // Print IP address and port
        std::cout << "Client IP: " << ip_str << std::endl;
        std::cout << "Client Port: " << ntohs(addr_in->sin_port) << std::endl;
    
    }
    else 
    {
        std::cerr << "Unsupported address family." << std::endl;
    }
}

void HttpServer::AccepteConnectionAndRecive()
{

    std::cout <<"waiting for conection on Port: " << PORT <<std::endl;
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    this->FdConnection = accept(ServerFd, (struct sockaddr *)&client_addr, &client_addrlen);
    // printClientInfo((struct sockaddr *)&client_addr, client_addrlen);
    if(FdConnection < 0)
    {
        throw std::runtime_error(std::string("accept:") + strerror(errno));
    }
    int r = read(FdConnection, this->RecivedRequest, MAXLEN - 1);
    if(r < 0)
    {
        throw std::runtime_error(std::string("reading from file:"));
    }
}

void HttpServer::AccepteMultipleConnectionAndRecive()
{
    int max_clients = 30;
    int client_socket[max_clients];

    //set of socket descriptors  

    int i = 0;
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    } 

    while(true)
    {
        fd_set readfds;    
        //clear the socket set  
        FD_ZERO(&readfds);   

        //add master socket to set  
        FD_SET(ServerFd, &readfds);   
        int max_fd = ServerFd;   

        int j = 0;
        while (j < max_clients) 
        {
            if (client_socket[j] > 0) 
                FD_SET(client_socket[j], &readfds);
            if (client_socket[j] > max_fd)
                max_fd = client_socket[j];
            j++;
        }

        // Wait for any activity on the sockets
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) 
        {
            throw std::runtime_error(std::string("select:") + strerror(errno));
        }

        // Handle incoming connection
        if (FD_ISSET(ServerFd, &readfds)) 
        {
            std::cout <<"waiting for conection on Port: " << PORT <<std::endl;
            struct sockaddr_in client_addr;
            socklen_t client_addrlen = sizeof(client_addr);
            this->FdConnection = accept(ServerFd, (struct sockaddr *)&client_addr, &client_addrlen);
            // printClientInfo((struct sockaddr *)&client_addr, client_addrlen);
            if(FdConnection < 0)
            {
                throw std::runtime_error(std::string("accept:") + strerror(errno));
            }
            // Add new socket to array of sockets
            for (int i = 0; i < max_clients; ++i) 
            {
                if (client_socket[i] == 0) 
                {
                    client_socket[i] = this->FdConnection;
                    break;
                }
                // Handle client sockets
                for (int i = 0; i < max_clients; ++i) 
                {
                    int new_sockets = client_socket[i];

                    if (FD_ISSET(new_sockets, &readfds)) 
                    {
                        // Handle read/write operations for client_socket
                        int valread = read(new_sockets, this->RecivedRequest, MAXLEN - 1);
                        if (valread <= 0) 
                        {
                            // Client disconnected or error
                            if (valread == 0)
                                std::cout << "Client disconnected, fd: " << client_socket << std::endl;
                            else
                                perror("read error");
                            close(new_sockets);
                            client_socket[i] = 0;
                        } 
                        else 
                        {
                            HttpRequest Request(this->GetRequest());
                            HttpResponse Response(Request);
                            this->SendMultiResponse(Response, new_sockets);
                        }
                    }
                }
            }
        }
    }
}

#define SP " "

void HttpServer::SendMultiResponse(const HttpResponse &Response, int fd) const
{
    std::string FinalResponse = Response.GetHttpVersion() + SP + Response.HTTPStatusCodeToString() + SP + Response.GetHttpStatusMessage() + "\r\n";
    std::list<KeyValue>::const_iterator it = Response.GetHeadersBegin();
    std::list<KeyValue>::const_iterator itend = Response.GetHeadersEnd();
    while(it != itend)
    {
        FinalResponse += it->HttpHeader + it->HttpValue + '\n';
        it++;
    }
    FinalResponse += "\r\n";
    FinalResponse += Response.GetResponseBody();
    
    int response_length = strlen(FinalResponse.c_str());
    if(write(fd, FinalResponse.c_str(), response_length) != response_length) 
    {
        throw std::runtime_error(std::string("write to FdConnection:"));
    }
    close(fd);
}

void HttpServer::SendResponse(const HttpResponse &Response) const
{
    std::string FinalResponse = Response.GetHttpVersion() + SP + Response.HTTPStatusCodeToString() + SP + Response.GetHttpStatusMessage() + "\r\n";
    std::list<KeyValue>::const_iterator it = Response.GetHeadersBegin();
    std::list<KeyValue>::const_iterator itend = Response.GetHeadersEnd();
    while(it != itend)
    {
        FinalResponse += it->HttpHeader + it->HttpValue + '\n';
        it++;
    }
    FinalResponse += "\r\n";
    FinalResponse += Response.GetResponseBody();
    
    int response_length = strlen(FinalResponse.c_str());
    if(write(FdConnection, FinalResponse.c_str(), response_length) != response_length) 
    {
        throw std::runtime_error(std::string("write to FdConnection:"));
    }
    close(FdConnection);
}


