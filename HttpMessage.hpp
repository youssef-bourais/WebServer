/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created1  2024/04/20 13:09:21 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/03 13:50:12 by ybourais         ###   ########.fr       */
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

void HttpMessage::PrintHeaders() const
{
    std::list<KeyValue>::const_iterator it = HttpHeaders.begin();
    while (it != HttpHeaders.end()) 
    {
        std::cout << it->HttpHeader<< ":"<< it->HttpValue<<std::endl;
        it++;
    }
}


HttpMessage::HttpMessage() : RecivedLine(""), HttpVersion(""), HttpHeaders(), Body("")
{
}

std::string HttpMessage::GetRecivedLine() const
{
    return this->RecivedLine;
}


std::string HttpMessage::GetHttpVersion() const
{
    return this->HttpVersion;
}

void HttpMessage::PrintRequest() const
{
    std::cout << std::endl<<"++++++++++++++++++| Full Request |++++++++++++++++"<<std::endl;
    std::cout << this->RecivedLine;
}

std::list<KeyValue> InitHttpheaders(std::string Line)
{
    std::list<KeyValue> headers;
    int start_pos = Line.find("\r\n") + 2;
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

HttpMessage &HttpMessage::operator=(const HttpMessage &src) 
{
    if(this != &src)
    {
        HttpHeaders.clear();
        std::list<KeyValue>::const_iterator it = src.HttpHeaders.begin();
        while(it != src.HttpHeaders.end())
        {
            HttpHeaders.push_back(*it); 
            it++;
        }
        this->RecivedLine = src.RecivedLine;
        this->HttpVersion = src.HttpVersion;
        this->Body = src.Body;
    }
    return *this;
}

HttpMessage::HttpMessage(const HttpMessage& copy) 
{
    *this = copy;
}

std::string GetVersion(std::string RecivedLine)
{
    int first_space = RecivedLine.find(' ');
    int start = RecivedLine.find(' ', first_space + 1);
    int end = RecivedLine.find('\n');
    std::string n = RecivedLine.substr(start + 1, end - start - 2);
    return n;
}

std::string getBody(std::string RecivedLine)
{
    int start = RecivedLine.find(CRLF);
    std::string body = RecivedLine.substr(start + 4);
    return body;
}

std::string HttpMessage::GetBody() const
{
    return this->Body;
}

int checkifbodyexist(std::list<KeyValue>::iterator begin, std::list<KeyValue>::iterator end)
{

    std::list<KeyValue>::iterator it = begin;
    while (it != end) 
    {
        if(it->HttpHeader == "Content-Length")
            return 1;
        it++;
    }
    return 0;
}

HttpMessage::HttpMessage(std::string RecivedLine)
{
    this->RecivedLine = RecivedLine; 
    this->HttpVersion = GetVersion(RecivedLine); 
    this->HttpHeaders = InitHttpheaders(RecivedLine); 
    if(checkifbodyexist(this->HttpHeaders.begin(), this->HttpHeaders.end())) 
        this->Body = getBody(RecivedLine);
    else
        this->Body = "";
}

HttpMessage::~HttpMessage()
{
}

