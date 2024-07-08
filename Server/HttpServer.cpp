/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 21:14:22 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/09 00:53:57 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <map>
#include <stdint.h> // Include for uint32_t definition
#include <sys/select.h>
/* #include <sys/_types/_fd_def.h> */
#include <sys/socket.h>
#include <sys/wait.h>
#include <vector>
#include "../Tools/Tools.hpp"


#define SP " "

// HttpServer::HttpServer(const ErrorsChecker &checker)
// {
//
//     //making the config data ready
//     Parsing parse(checker.GetConfigFilePath());
//     this->ServerSetting = parse.getServers();
//
//     // PF_INET: ipv4 or (AF_UNSPEC) for both
//     // SOCK_STREAM: TCP
//     for (int i = 0; i < ServerSetting.size();i++) 
//     {
//
//     }
//     this->ServerFd = socket(AF_INET, SOCK_STREAM, 0);
//     this->FdConnection = 0;
//     if (ServerFd  == -1) 
//     { 
//         throw std::runtime_error(std::string("Socket: ") + strerror(errno));
//     }
//     memset(&Address, 0,sizeof(Address));
//     Address.sin_family = AF_INET; // ipv4
//     Address.sin_addr.s_addr = htonl(INADDR_ANY); // any availabe local iP, we use htonl to convert the INADDR_ANY constant to network byte order
//     Address.sin_port = htons(PORT); // 
//     memset(RecivedRequest, 0, MAXLEN);
//
// }




void HttpServer::SetFdToNonBlocking()
{

    int flags = fcntl(this->ServerFd, F_GETFL, 0);
    if (flags == -1) 
    {
        throw std::runtime_error(std::string("fcntl: ") + strerror(errno));
    }
    if (fcntl(this->ServerFd, F_SETFL, flags | O_NONBLOCK) == -1) 
    {
        throw std::runtime_error(std::string("fcntl: ") + strerror(errno));
    }
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

void setNonBlocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) 
    {
        throw std::runtime_error(std::string("fcntl: ") + strerror(errno));
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) 
    {
        throw std::runtime_error(std::string("fcntl: ") + strerror(errno));
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



int HttpServer::AccepteConnection(int fd)
{   
    static int a;
    std::cout <<"waiting for conection on Port: " << PORT << " :"<< a++<<std::endl;
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    int neww = accept(fd, (struct sockaddr *)&client_addr, &client_addrlen);
    // this->FdConnection = accept(ServerFd, (struct sockaddr *)&client_addr, &client_addrlen);
    // printClientInfo((struct sockaddr *)&client_addr, client_addrlen);
    if(neww < 0)
    {
        throw std::runtime_error(std::string("accept:") + strerror(errno));
    }
    return neww;
}

int HttpServer::AccepteConnection()
{   
    static int a;
    std::cout <<"waiting for conection on Port: " << PORT << " :"<< a++<<std::endl;
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    int neww = accept(ServerFd, (struct sockaddr *)&client_addr, &client_addrlen);
    // this->FdConnection = accept(ServerFd, (struct sockaddr *)&client_addr, &client_addrlen);
    // printClientInfo((struct sockaddr *)&client_addr, client_addrlen);
    if(neww < 0)
    {
        throw std::runtime_error(std::string("accept:") + strerror(errno));
    }
    return neww;
}


const std::string HttpServer::ReciveData(int fd)
{    
    int r = recv(fd, this->RecivedRequest, MAXLEN - 1, 0);
    if(r < 0)
    {
        std::cout << "errrrrr"<<std::endl;
        throw std::runtime_error(std::string("recv: "));
    }
    return this->RecivedRequest;
}

const std::string HttpServer::GenarateResponse(const HttpResponse &Response, int fd) const
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
    return FinalResponse;
}

ssize_t HttpServer::SendMultiResponse(const HttpResponse &Response, int fd, int *helper) const
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
    *helper = response_length;
    ssize_t sendingbyt;
    sendingbyt = send(fd, FinalResponse.c_str(), response_length, 0) ;
    if(sendingbyt != response_length) 
    {
        throw std::runtime_error(std::string("send to FdConnection:"));
    }
    return sendingbyt;
}

HttpServer::HttpServer(const ErrorsChecker &checker)
{
    //making the config data ready
    Parsing parse(checker.GetConfigFilePath());
    this->ServerSetting = parse.getServers();
    int fd;

    // PF_INET: ipv4 or (AF_UNSPEC) for both
    // SOCK_STREAM: TCP
    for (int i = 0; i < ServerSetting.size();i++) 
    {
        std::vector<int> ports;
        for (int j = 0; j < ServerSetting[i].listen.size();j++) 
        {
            fd = socket(AF_INET, SOCK_STREAM, 0);
            if (ServerFd  == -1) 
            { 
                throw std::runtime_error(std::string("Socket: ") + strerror(errno));
            }
            memset(&Address, 0,sizeof(Address));
            Address.sin_family = AF_INET; // ipv4
            Address.sin_addr.s_addr = htonl(INADDR_ANY); // any availabe local iP, we use htonl to convert the INADDR_ANY constant to network byte order
            Address.sin_port = htons(StringToInt(ServerSetting[i].listen[j])); // 
            memset(RecivedRequest, 0, MAXLEN);

            int opt = 1;
            // it helps in reuse of address and port. Prevents error such as: “address already in use”
            if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
            {
                throw std::runtime_error(std::string("setsockopt:") + strerror(errno));
            }

            SetSocketToNonBlocking(fd);

            if(bind(fd, (struct sockaddr *)&Address, sizeof(Address)) < 0)
            {
                throw std::runtime_error(std::string("bind: ") + strerror(errno));
            }


            //socket turn into passive listening state

            if(listen(fd, 1024) < 0)
            {
                throw std::runtime_error(std::string("listen:") + strerror(errno));
            }
            ports.push_back(fd);
        }
        this->Fds.push_back(ports);
    }
}


void HttpServer::AccepteMultipleConnectionAndRecive()
{
    int activity;
    int max_fd;
    fd_set current_set, ready_set;

    fd_set ready_readfds, ready_writefds, current_readfds, current_writefds;

    FD_ZERO(&current_readfds);
    FD_ZERO(&current_writefds);
    // FD_SET(this->ServerFd, &current_readfds);

    for (int i = 0;i < this->Fds.size();i++) 
    {
        for (int j = 0; j < this->Fds[i].size(); j++) 
        {
            FD_SET(this->Fds[i][j], &current_readfds);
            if(this->Fds[i][j] > max_fd)
                max_fd = this->Fds[i][j];
        }
    }

    struct timeval      timeout;

    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    std::map<int , std::string> buffers;
    std::map<int, std::string> ResponseMsg;
    std::vector<int> allSocket;
    std::vector<int> ListingSocket;
    std::vector<int> WritingSocket;

    for(int i = 0;i < this->Fds.size();i++)
    {
        for (int j = 0; j < this->Fds[i].size();j++) 
        {
            allSocket.push_back(this->Fds[i][j]);
            ListingSocket.push_back(this->Fds[i][j]);
        }
    }


    while(true)
    {
        ready_readfds = current_readfds; 
        ready_writefds = current_writefds;

        activity = select(max_fd + 1, &ready_readfds, &ready_writefds, NULL, NULL);
        if(activity < 0)
            throw std::runtime_error(std::string("selcet:") + strerror(errno));
        if(activity == 0)
            continue;

        for (int i = 0;i < allSocket.size();i++) 
        {
            int fd = allSocket[i];
            if(FD_ISSET(fd, &ready_readfds))
            {
                std::cout << "==================="<<std::endl;
                std::cout << "some fd in the all fd list: "<< fd<<std::endl;
                std::vector<int>::iterator it = find(ListingSocket.begin(), ListingSocket.end(), fd);
                //checking if the fd is in the Listing vec (if fd is a new connection ) Listing socket list contain all server socket
                if(it != ListingSocket.end())
                {
                    std::cout << "++++++++++++"<<std::endl;
                    std::cout << "socket fd : "<< fd<<std::endl;
                    int new_fd = this->AccepteConnection(fd);
                    std::cout << "client fd to read from it later: "<< new_fd<<std::endl;
                    setNonBlocking(new_fd);
                    FD_SET(new_fd, &current_readfds);
                    FD_SET(new_fd, &current_writefds);
                    if (new_fd > max_fd) 
                    {
                        max_fd = new_fd;
                    }
                    allSocket.push_back(new_fd);
                    for (int i = 0;i < allSocket.size();i++) 
                    {
                         std::cout << allSocket[i]<<std::endl;

                    }
                }
                // reading operation
                else 
                {
                    std::cout << "-----------------"<<std::endl;
                    std::cout << "cliet fd to read : "<<fd<<std::endl;
                    //reding data from the socket of the client
                    buffers[fd] = this->ReciveData(fd);
                    FD_SET(fd, &current_writefds);
                    WritingSocket.push_back(fd);
                }
            }
        }

        // loping and checking if some fd in the writing set is ready to write to its fd (send respose to socket client)
        for (int j = 0;j < WritingSocket.size();j++) 
        {
            int fd = WritingSocket[j];
            if (FD_ISSET(fd, &ready_writefds)) // indicates that the socket's send buffer has space available to accept more data
            {
                if(ResponseMsg.find(fd) == ResponseMsg.end())
                {
                    HttpRequest Request(buffers[fd]);
                    HttpResponse Response(Request);
                    ResponseMsg[fd] = this->GenarateResponse(Response, fd);
                }

                std::string &message = ResponseMsg[fd];
                int bytes_sent = this->SendChunckedResponse(fd, message);
                std::cout << "bytes_sent: " << bytes_sent <<std::endl;
                if (bytes_sent < 0)
                {
                    close(fd);
                    FD_CLR(fd, &current_readfds);
                    FD_CLR(fd, &current_writefds);
                    WritingSocket.erase(WritingSocket.begin() + j);
                    j--;
                    ResponseMsg.erase(fd);
                }
                else
                {
                    message.erase(0, bytes_sent); // Remove sent bytes from buffer
                    if (message.empty())
                    {
                        // Entire response has been sent
                        close(fd);
                        FD_CLR(fd, &current_readfds);
                        FD_CLR(fd, &current_writefds);
                        WritingSocket.erase(WritingSocket.begin() + j);
                        std::vector<int>::iterator it = find(allSocket.begin(), allSocket.end(), fd);
                        std::vector<int>::iterator i = allSocket.begin();
                        int k = 0;
                        while (i != allSocket.end())
                        {
                            if(it == i)
                                break;
                            i++;
                            k++;

                        }
                        allSocket.erase(allSocket.begin() + k);
                        j--;
                        ResponseMsg.erase(fd);
                    }
                }
            }
        }
    }
}

// void HttpServer::AccepteMultipleConnectionAndRecive()
// {
//     int activity;
//     int max_fd;
//     fd_set current_set, ready_set;
//
//     fd_set ready_readfds, ready_writefds, current_readfds, current_writefds;
//
//     FD_ZERO(&current_readfds);
//     FD_ZERO(&current_writefds);
//     FD_SET(this->ServerFd, &current_readfds);
//
//     // for (int i = 3;i < this->Fds.size();i++) 
//     // {
//     //     for (int j = 0; j < this->Fds[i].size(); j++) 
//     //     {
//     //         FD_SET(this->Fds[i][j], &current_readfds);
//     //         if(this->Fds[i][j] > max_fd)
//     //             max_fd = this->Fds[i][j];
//     //     }
//     // }
//     max_fd = this->ServerFd;
//
//     struct timeval      timeout;
//
//     timeout.tv_sec  = 1;
//     timeout.tv_usec = 0;
//
//     std::vector<int > Fds;
//     std::map<int , std::string> buffers;
//     std::map<int, std::string> ResponseMsg;
//     while(true)
//     {
//         ready_readfds = current_readfds; ready_writefds = current_writefds;
//
//         activity = select(max_fd + 1, &ready_readfds, &ready_writefds, NULL, NULL);
//         if(activity < 0)
//             throw std::runtime_error(std::string("selcet:") + strerror(errno));
//         if(activity == 0)
//             continue;
//
//         //loping to accepte new fd and read from them
//         for(int i = this->ServerFd;i <= max_fd;i++)
//         {
//             if(FD_ISSET(i, &ready_readfds))
//             {
//                 // new connection
//                 if(i == this->ServerFd)
//                 {
//                     int new_fd = this->AccepteConnection();
//                     setNonBlocking(new_fd);
//                     FD_SET(new_fd, &current_readfds);
//                     FD_SET(new_fd, &current_writefds);
//                     if (new_fd > max_fd) 
//                         max_fd = new_fd;
//                 }
//                 // reading operation
//                 else 
//                 {
//                     //reding data from the socket of the client
//                     buffers[i] = this->ReciveData(i);
//                     Fds.push_back(i);
//                     FD_SET(i, &current_writefds);
//                 }
//             }
//         }
//         // loping and checking if some fd in the writing set is ready to write to its fd (send respose to socket client)
//         for (int j = 0;j < Fds.size();j++) 
//         {
//             int fd = Fds[j];
//             if (FD_ISSET(fd, &ready_writefds)) // indicates that the socket's send buffer has space available to accept more data
//             {
//                 if(ResponseMsg.find(fd) == ResponseMsg.end())
//                 {
//                     HttpRequest Request(buffers[fd]);
//                     HttpResponse Response(Request);
//                     ResponseMsg[fd] = this->GenarateResponse(Response, fd);
//                 }
//
//                 std::string &message = ResponseMsg[fd];
//                 int bytes_sent = this->SendChunckedResponse(fd, message);
//                 std::cout << bytes_sent <<std::endl;
//                 if (bytes_sent < 0)
//                 {
//                     close(fd);
//                     FD_CLR(fd, &current_readfds);
//                     FD_CLR(fd, &current_writefds);
//                     Fds.erase(Fds.begin() + j);
//                     j--;
//                     ResponseMsg.erase(fd);
//                 }
//                 else
//                 {
//                     message.erase(0, bytes_sent); // Remove sent bytes from buffer
//                     if (message.empty())
//                     {
//                         // Entire response has been sent
//                         close(fd);
//                         FD_CLR(fd, &current_readfds);
//                         FD_CLR(fd, &current_writefds);
//                         Fds.erase(Fds.begin() + j);
//                         j--;
//                         ResponseMsg.erase(fd);
//                     }
//                 }
//             }
//         }
//     }
// }

int HttpServer::SendChunckedResponse(int fd, std::string &message)
{
    int bytes_sent = send(fd, message.c_str(), message.size(), 0);
    return bytes_sent;
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


