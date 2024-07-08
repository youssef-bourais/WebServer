/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 20:42:24 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/08 00:47:10 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <list>
#include <netinet/in.h> // struct sockaddr_in
#include <sys/errno.h>
#include <sys/socket.h>
#include <iostream>
 #include <unistd.h>
#include "../parsing/parsingStruct.hpp"

#include "../Request/HttpMessage.hpp"
#include "../Response/HttpResponse.hpp"

#include "../errors/Errors.hpp"

#define MAXLEN 8192
#define SA struct sockaddr_in
#define PORT 80

class HttpServer
{
    private:
        int ServerFd;
        int FdConnection;
        SA Address;
        char RecivedRequest[MAXLEN + 1];
        std::vector<t_servers> ServerSetting;
        
    public:
        HttpServer(const ErrorsChecker &checker);
        ~HttpServer();
        HttpServer &operator=(HttpServer const &s);
        HttpServer(HttpServer const &src);

        std::string GetRequest() const;

        void ForceReuse() const;
        void SetFdToNonBlocking();
        void BindSocketToAddr() const;
        void StartListining(int PendingConection) const;
        void AccepteConnectionAndRecive();

        int AccepteConnection();
        const std::string ReciveData(int fd);
        const std::string GenarateResponse(const HttpResponse &Response, int fd) const;
        int SendChunckedResponse(int fd, std::string &message);

        void AccepteMultipleConnectionAndRecive();

        void SendResponse(HttpResponse const &Response) const;
        ssize_t SendMultiResponse(HttpResponse const &Response, int fd, int *helper) const;
};


