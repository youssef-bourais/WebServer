/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:27:28 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/19 17:13:22 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

std::list<KeyValue> InitHttpheaders(std::string Line)
{
    std::list<KeyValue> headers;
    int start_pos = Line.find("\r\n") + 1;
    std::string tmp;

    while (start_pos < (int)Line.length()) 
    {
        if(Line[start_pos] == '\r')
        {
            start_pos += 2;
            unsigned long del = tmp.find(":");
            if(del == std::string::npos)
                continue;
            std::string key = tmp.substr(0, del);
            std::string value = tmp.substr(del + 2, tmp.length());
            headers.push_back(KeyValue(key,value));
            tmp = "";
            continue;
        }
        tmp += Line[start_pos];
        start_pos++;
    }
    return headers;
}

HttpRequest::HttpRequest()
{
}


HttpRequest::HttpRequest(std::string RecivedLine)
{
    this->HttpHeaders = InitHttpheaders(RecivedLine);
    this->RecivedLine = RecivedLine;
}


HttpRequest::~HttpRequest()
{

}

HttpRequest &HttpRequest::operator=(const HttpRequest &s) 
{
    if(this != &s)
    {
        HttpHeaders.clear();
        std::list<KeyValue>::const_iterator it = s.HttpHeaders.begin();
        while(it != s.HttpHeaders.end())
        {
            HttpHeaders.push_back(*it); 
            it++;
        }
        RecivedLine = s.RecivedLine;
    }
    return *this;
}

HttpRequest::HttpRequest(const HttpRequest& copy) 
{
    *this = copy;
}



void HttpRequest::PrintHeaders() const
{
    std::list<KeyValue>::const_iterator it = HttpHeaders.begin();
    while (it != HttpHeaders.end()) 
    {
        std::cout << it->HttpHeader<< ":"<< it->HttpValue<<std::endl;
        it++;
    }
}

const std::string HttpRequest::HttpMethod() const
{
    int pos = RecivedLine.find(' ');
    std::string Method = RecivedLine.substr(0, pos); 
    return(Method);
}


const std::string HttpRequest::Path() const
{
    int first_space = RecivedLine.find(' ');
    int second_space = RecivedLine.find(' ', first_space + 1);
    std::string path = RecivedLine.substr(first_space + 1, second_space - first_space - 1);

    return path;

}

void HttpRequest::PrintRequest() const
{
    std::cout << this->RecivedLine;
}

const std::string HttpRequest::HttpVersion() const
{
    int first_space = RecivedLine.find(' ');
    int start = RecivedLine.find(' ', first_space + 1);
    int end = RecivedLine.find('\n');
    std::string n = RecivedLine.substr(start + 1, end - start - 2);
    return n;
}
