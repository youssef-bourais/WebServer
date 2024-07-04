/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 21:14:22 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/04 23:10:49 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"
#include <cstdlib>
#include <stdint.h> // Include for uint32_t definition
#include <sys/_select.h>
#include <sys/_types/_fd_def.h>
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

void HttpServer::SetFdToNonBlocking()
{

    int flags = fcntl(this->ServerFd, F_GETFL, 0);
    if (flags == -1) 
        throw std::runtime_error(std::string("fcntl :") + strerror(errno));
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
    int r = recv(FdConnection, this->RecivedRequest, MAXLEN - 1, 0);
    if(r < 0)
    {
        throw std::runtime_error(std::string("reaciving from file:"));
    }
}

// void HttpServer::AccepteMultipleConnectionAndRecive()
// {
//     int max_clients = 30;
//     int client_socket[max_clients];
//
//     //set of socket descriptors  
//
//     int i = 0;
//     for (i = 0; i < max_clients; i++)   
//     {   
//         client_socket[i] = 0;   
//     } 
//
//     while(true)
//     {
//         fd_set readfds;    
//
//         //clear the socket set  
//         FD_ZERO(&readfds);   
//
//         //add master socket to set  
//         FD_SET(ServerFd, &readfds);   
//         int max_fd = ServerFd;   
//
//         int j = 0;
//         while (j < max_clients) 
//         {
//             if (client_socket[j] > 0) 
//                 FD_SET(client_socket[j], &readfds);
//             if (client_socket[j] > max_fd)
//                 max_fd = client_socket[j];
//             j++;
//         }
//
//         // Wait for any activity on the sockets
//         int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
//         if ((activity < 0) && (errno != EINTR)) // forbiding
//         {
//             throw std::runtime_error(std::string("select:") + strerror(errno));
//         }
//
//         // Handle incoming connection
//         if (FD_ISSET(ServerFd, &readfds)) 
//         {
//             std::cout <<"waiting for conection on Port: " << PORT <<std::endl;
//             this->AccepteConnectionAndRecive();
//
//             // Add new socket to array of sockets
//             for (int i = 0; i < max_clients; ++i) 
//             {
//                 if (client_socket[i] == 0) 
//                 {
//                     client_socket[i] = FdConnection;
//                     break;
//                 }
//                 // Handle client sockets
//                 for (int i = 0; i < max_clients; ++i) 
//                 {
//                     int new_sockets = client_socket[i];
//
//                     if (FD_ISSET(new_sockets, &readfds)) 
//                     {
//                         // Handle read/write operations for client_socket
//                         // int valread = read(new_sockets, this->RecivedRequest, MAXLEN - 1);
//                         int valread = recv(new_sockets, this->RecivedRequest, MAXLEN - 1, 0);
//                         if (valread <= 0) 
//                         {
//                             // Client disconnected or error
//                             if (valread == 0)
//                                 std::cout << "Client disconnected, fd: " << client_socket << std::endl;
//                             else
//                                 perror("read error");
//                             close(new_sockets);
//                             client_socket[i] = 0;
//                         } 
//                         else 
//                         {
//                             std::cout << this->GetRequest()<<std::endl;
//                             exit(0);
//                             HttpRequest Request(this->GetRequest());
//                             HttpResponse Response(Request);
//                             this->SendMultiResponse(Response, new_sockets);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

void HttpServer::AccepteConnection()
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
}

void HttpServer::ReciveData()
{    
    int r = recv(this->FdConnection, this->RecivedRequest, MAXLEN - 1, 0);
    if(r < 0)
    {
        throw std::runtime_error(std::string("reaciving from file:"));
    }
}


void HttpServer::AccepteMultipleConnectionAndRecive()
{
    int activity;
    int max_fd;
    int max_client = 30;
    fd_set current_set, ready_set;

    //init the set of fd
    FD_ZERO(&current_set);
    FD_SET(this->ServerFd, &current_set);

    struct timeval      timeout;
    timeout.tv_sec  = 3 * 60;
    timeout.tv_usec = 0;

    while(true)
    {
        ready_set = current_set;

        max_fd = ServerFd;   
        if(activity == select(FD_SETSIZE + 1, &ready_set, NULL, NULL, NULL) < 0)
            throw std::runtime_error(std::string("selcet:") + strerror(errno));
        for(int i = 0;i < max_client;i++)
        {
            if(FD_ISSET(i, &ready_set))
            {
                //new connection
                if(i == this->ServerFd)
                {
                    this->AccepteConnection();

                    this->ReciveData();
                    HttpRequest Request(this->GetRequest());

                    HttpResponse Response(Request);
                    this->SendResponse(Response);

                    FD_SET(FdConnection, &current_set);
                }
                else 
                {
                    this->AccepteConnection();
                    this->ReciveData();
                    HttpRequest Request(this->GetRequest());
                    //
                    HttpResponse Response(Request);
                    this->SendMultiResponse(Response, i);
                    FD_CLR(i, &current_set);
                }
            }
        }
    }

}


// void HttpServer::AccepteMultipleConnectionAndRecive()
// {
//
//     fd_set  master_set, working_set;
//     int max_sd;
//     int close_con;
//
//    /* Initialize the master fd_set */
//     FD_ZERO(&master_set);
//     max_sd = this->ServerFd;
//     FD_SET(this->ServerFd, &master_set);
//
//
//    /* Initialize the timeval struct to 3 minutes */
//    struct timeval      timeout;
//    timeout.tv_sec  = 3 * 60;
//    timeout.tv_usec = 0;
//
//    int    desc_ready, end_server = false;
//
//    /* Loop waiting for incoming connects or for incoming data */
//    /* on any of the connected sockets.*/
//     while(true)
//     {
//
//         /* Copy the master fd_set over to the working fd_set.     */
//         memcpy(&working_set, &master_set, sizeof(master_set));
//
//         /* Call select() and wait 3 minutes for it to complete.   */
//         int rc = select(max_sd + 1, &working_set, NULL, NULL, NULL);
//         if (rc < 0)
//             throw std::runtime_error(std::string("select:"));
//
//         /* Check to see if the 3 minute time out expired.         */
//         if (rc == 0)
//             throw std::runtime_error(std::string("select:timeout:"));
//
//         /* One or more descriptors are readable.  Need to         */
//         /* determine which ones they are.                         */
//         int i = 0;
//         int desc_ready = rc;
//         while(i <= max_sd && desc_ready > 0)
//         {
//
//             /* Check to see if this descriptor is ready            */
//             if (FD_ISSET(i, &working_set))
//             {
//
//                 /* A descriptor was found that was readable - one   */
//                 /* less has to be looked for.  This is being done   */
//                 /* so that we can stop looking at the working set   */
//                 /* once we have found all of the descriptors that   */
//                 /* were ready.                                      */
//                 desc_ready -= 1;
//
//                 /* Check to see if this is the listening socket     */
//                 if (i == this->ServerFd)
//                 {
//                     std::printf("Listening socket is readable\n");
//
//
//                     /* Loop and accept another incoming conncection */
//                     // while(FdConnection != 1)
//                     // {
//
//                         /* Accept all incoming connections that are      */
//                         /* queued up on the listening socket before we   */
//                         /* loop back and call select again.              */
//                         this->AccepteConnectionAndRecive();
//                     // }
//
//                     /* Add the new incoming connection to the     */
//                     /* master read set                            */
//                     std::printf("New incoming connection - %d\n", this->FdConnection);
//                     FD_SET(this->FdConnection, &master_set);
//                     if(this->FdConnection > max_sd)
//                         max_sd = this->FdConnection;
//
//                 }
//
//                 /* This is not the listening socket, therefore an   */
//                 /* existing connection must be readable             */
//                 else 
//                 {
//
//                     std::printf("  Descriptor %d is readable\n", i);
//                     close_con = false;
//                     while (true) 
//                     {
//
//
//                         /* Receive all incoming data on this socket      */
//                         /* before we loop back and call select again.    */
//                         HttpRequest Request(this->GetRequest());
//                         HttpResponse Response(Request);
//                         this->SendMultiResponse(Response, i);
//                         break;
//                     }
//                     if(close_con)
//                     {
//                         close(i);
//                         FD_CLR(i, &master_set);
//                         if(i == max_sd)
//                         {
//                             while(FD_ISSET(max_sd, &master_set) == false)
//                                 max_sd -= 1;
//                         }
//                     }
//
//                 } /* End of existing connection is readable */
//
//
//             }/*End of if (FD_ISSET(i, &working_set)) */
//
//         } /* End of loop through selectable descriptors */
//     }
//
//     ///close all
//     for (int i = 0; i <= max_sd; ++i)
//     {
//       if (FD_ISSET(i, &master_set))
//          close(i);
//     }
// }

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
    if(send(fd, FinalResponse.c_str(), response_length, 0) != response_length) 
    {
        throw std::runtime_error(std::string("send to FdConnection:"));
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
    // if(send(FdConnection, FinalResponse.c_str(), response_length, MSG_DONTWAIT) != response_length) 
    if(send(FdConnection, FinalResponse.c_str(), response_length, 0) != response_length) 
    {
        throw std::runtime_error(std::string("send to FdConnection:"));
    }
    close(FdConnection);
}


