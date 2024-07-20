/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 20:42:24 by ybourais          #+#    #+#             */
/*   Updated: 2024/07/20 17:44:58 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "../parsing/parsingStruct.hpp"
#include "../Request/RequestParsser.hpp"
#include "../Response/HttpResponse.hpp"
#include "../errors/Errors.hpp"
#include <netinet/in.h> // struct sockaddr_in
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <stdint.h> // Include for uint32_t definition
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <vector>



#define MAXLEN 8192
#define SA struct sockaddr_in
#define PORT 80

class HttpServer
{
    private:
        SA Address;
        char RecivedRequest[MAXLEN + 1];
        std::vector<t_servers> ServerSetting;
        std::vector<std::vector<int> > Fds;
        
    public:
        HttpServer(const ErrorsChecker &checker);
        ~HttpServer();
        HttpServer &operator=(HttpServer const &s);
        HttpServer(HttpServer const &src);

        std::string GetRequest() const;


        void AccepteMultipleConnectionAndRecive();
        RequestParsser ReciveData(int fd);
        const std::string GenarateResponse(const HttpResponse &Response, int fd) const;
        int SendResponse(int fd, std::string &message);
        
        int AccepteConnection(int fd);
};


