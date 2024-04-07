/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 20:35:47 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/06 21:26:04 by ybourais         ###   ########.fr       */
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
        HttpRequest(std::string RecivedLine);
        ~HttpRequest();
        HttpRequest &operator=(HttpRequest const &s);//operator overload
        HttpRequest(HttpRequest const &src);     
        
        const std::string HttpMethod() const;
        const std::string Path() const;
        const std::string HttpVersion() const;
        
        void PrintHttpHeaders() const;
    

};


HttpRequest::HttpRequest(std::string request)
{
    this->RecivedLine = request;

    std::string::size_type startPos = request.find("\n") + 1;
    std::string::size_type endPos;

    while ((endPos = request.find("\r\n", startPos)) != std::string::npos) 
    {
        std::string line = request.substr(startPos, endPos - startPos);
        std::cout << line<<std::endl;
        /* HttpHeaders.push_back(KeyValue(line.substr(0, line.find(":")), line.substr(line.find(":"),line.length()))); */
        startPos = endPos + 2;
    }
    if (startPos < request.length()) 
    {
        std::string line = request.substr(startPos);
        std::cout << line<<std::endl;
        /* std::cout << line<<std::endl; */
        /* HttpHeaders.push_back(KeyValue(line.substr(0, line.find(":")), line.substr(line.find(":"),line.length()))); */
    }
}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::PrintHttpHeaders() const
{
    std::list<KeyValue>::const_iterator it;
    it = HttpHeaders.begin();
    while (it != HttpHeaders.end()) 
    {
        std::cout << it->HttpHeader << ":"<< it->HttpValue<<std::endl;
        it++;
    }

}
