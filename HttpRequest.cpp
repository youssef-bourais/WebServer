/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:27:28 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/04 15:39:57 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::~HttpRequest()
{
}

std::string GetMethod(std::string RecivedLine)
{
    int pos = RecivedLine.find(' ');
    std::string Method = RecivedLine.substr(0, pos);
    return(Method);
}


void NormalizePath(std::string &Path)
{

    std::string Cor;
    int i = 0;
    while (i < (int)Path.length()) 
    {
        Cor += Path[i];
        if (Path[i] == '/') 
        {
            while (Path[i] == '/') 
                i++;
            continue;
        }
        i++;
    }
    Path = Cor;
}

std::string getPath(std::string RecivedLine)
{
    int first_space = RecivedLine.find(' ');
    int second_space = RecivedLine.find(' ', first_space + 1);
    std::string path = RecivedLine.substr(first_space + 1, second_space - first_space - 1);
    NormalizePath(path);
    return path;
}

std::string HttpRequest::GetPath() const
{
    return this->Path;
}

std::string HttpRequest::GetHttpMethod() const
{
    return HttpMethod;
}

HttpRequest::HttpRequest(std::string RecivedLine) : HttpMessage(RecivedLine)
{
    this->HttpMethod = GetMethod(RecivedLine);
    this->Path = getPath(RecivedLine);
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
        this->RecivedLine = s.RecivedLine;
        this->HttpVersion = s.HttpVersion;
        this->Body = s.Body;
        this->HttpMethod = s.HttpMethod;
        this->Path = s.Path;
    }
    return *this;
}

HttpRequest::HttpRequest(const HttpRequest& copy) : HttpMessage(copy)
{
    *this = copy;
}
