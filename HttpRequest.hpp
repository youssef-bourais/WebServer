/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 20:38:06 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/07 23:34:05 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <list>
#define MAXLEN 8024

/*parssing the request*/
/* HTTP method, requested URL, headers, and request body */

struct KeyValue 
{
    std::string HttpHeader;
    std::string HttpValue;
    KeyValue(const std::string &k, const std::string &v) : HttpHeader(k), HttpValue(v) {}
};

class HttpRequest
{
    private:
        std::list<KeyValue> HttpHeaders;
        std::string RecivedLine;
    public:
        HttpRequest();
        HttpRequest(std::string RecivedLine);
        ~HttpRequest();
        HttpRequest &operator=(HttpRequest const &s);
        HttpRequest(HttpRequest const &src);
        
        const std::string HttpMethod() const;
        const std::string Path() const;
        const std::string HttpVersion() const;

};


HttpRequest::HttpRequest(std::string RecivedLine)
{
    int start_pos = RecivedLine.find("\r\n") + 1;
    std::string tmp;
    while (start_pos < (int)RecivedLine.length()) 
    {
        if(RecivedLine[start_pos] == '\r')
        {

            start_pos += 2;
            unsigned long del = tmp.find(":");
            if(del == std::string::npos)
                continue;
            std::string key = tmp.substr(0, del);
            std::string value = tmp.substr(del + 1, tmp.length());
            std::cout << key<< " "<< value<< std::endl;

            tmp = "";
            continue;
        }
        tmp += RecivedLine[start_pos];
        start_pos++;

    }

}

HttpRequest::~HttpRequest()
{

}
