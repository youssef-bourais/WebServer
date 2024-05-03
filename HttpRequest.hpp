/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 20:38:06 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/03 14:14:28 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define MAXLEN 8192

#include "HttpMessage.hpp"

class HttpRequest : public HttpMessage
{
    private:
        std::string HttpMethod;
        std::string Path;
    public:
        HttpRequest(std::string RecivedLine);
        ~HttpRequest();
        HttpRequest &operator=(const HttpRequest &s);
        HttpRequest(const HttpRequest &copy);

        std::string GetHttpMethod() const;
        std::string GetPath() const;
};

HttpRequest::~HttpRequest()
{
}

std::string GetMethod(std::string RecivedLine)
{
    int pos = RecivedLine.find(' ');
    std::string Method = RecivedLine.substr(0, pos);
    return(Method);
}

#include <sys/stat.h>

int FileOrDir(std::string Path)
{
    struct stat s;
    if( stat(Path.c_str(),&s) == 0)
    {
        if( s.st_mode & S_IFDIR)
        {
            // it's a directory
            return 1;
        }
        else if( s.st_mode & S_IFREG)
        {
            // it's a file
            return 0;
        }
    }
    else
    {
        // error
        return -1;
    }
}

void NormalizePath(std::string &Path)
{

    std::string Cor;
    int i = 0;
    while (Path[i]) 
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


