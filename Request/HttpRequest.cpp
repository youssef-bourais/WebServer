/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:27:28 by ybourais          #+#    #+#             */
/*   Updated: 2024/06/27 19:46:37 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "../Tools/Tools.hpp"


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

void ParssPath(std::string &path)
{
    std::cout << path<<std::endl;
    NormalizePath(path);
    if(path.length() == 1)
        return;
    else 
    {
        std::string tmp = path.substr(1); // remove the first slash /
        int value = checkFileType(tmp);
        if(value == FILE_TYPE)
        {
            path = tmp;
            return ;
        }
        else if(value == DIR_TYPE)
        {
            if(tmp.back() != '/')
                tmp += '/';
            path = tmp;
            return ;
        }
        else 
        {
            if (!tmp.empty() && tmp.back() == '/') 
                tmp.pop_back();
            value = checkFileType(tmp);
            if(value == FILE_TYPE)
                path = tmp;
            else 
                path = "";
        }
    }
}

std::string getPath(std::string RecivedLine)
{
    int first_space = RecivedLine.find(' ');
    int second_space = RecivedLine.find(' ', first_space + 1);
    std::string path = RecivedLine.substr(first_space + 1, second_space - first_space - 1);
    ParssPath(path);
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
