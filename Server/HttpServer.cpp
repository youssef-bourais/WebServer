/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 21:14:22 by ybourais          #+#    #+#             */
/*   Updated: 2024/06/09 17:27:36 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"


HttpServer::HttpServer()
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
    /* this->Request = new HttpRequest(); */
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
    if(listen(ServerFd, PendingConection) < 0)
    {
        throw std::runtime_error(std::string("listen:") + strerror(errno));
    }
}

void HttpServer::AccepteConnectionAndRecive()
{
    std::cout <<"waiting for conection on Port: " << PORT <<std::endl;
    this->FdConnection = accept(ServerFd, (struct sockaddr *)NULL, NULL);
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

#define SP " "

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


