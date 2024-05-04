/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created1  2024/04/20 13:09:21 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/04 15:39:46 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <list>

#define CRLF "\r\n\r\n"

struct KeyValue 
{
    std::string HttpHeader;
    std::string HttpValue;
    KeyValue(const std::string &k, const std::string &v) : HttpHeader(k), HttpValue(v) {}
};

class HttpMessage
{
    protected:
        std::string RecivedLine;
        std::string HttpVersion;
        std::list<KeyValue> HttpHeaders;
        std::string Body;
    public:
        HttpMessage(std::string RecivedLine);
        HttpMessage();
        ~HttpMessage();
        HttpMessage(const HttpMessage &copy);
        HttpMessage &operator=(const HttpMessage &src);

        std::string GetBody() const;
        std::string GetHttpVersion() const;
        std::string GetRecivedLine() const;
        
        void PrintRequest() const;
        void PrintHeaders() const;
};



