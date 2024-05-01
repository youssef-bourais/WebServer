/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:36 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/01 18:21:14 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpMessage.hpp"
#include "HttpRequest.hpp"
#include <cstring>
#include <sys/fcntl.h>

/*response headers Server, Date, Content-Length, and Content-Type, Allow*/

enum HTTPStatusCode 
{
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_NO_CONTENT = 204,
    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_INTERNAL_SERVER_ERROR = 500
};


#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

class HttpResponse : public HttpMessage
{
    private:
        HTTPStatusCode StatusCode;
        std::string ResponseBody;
    public:
        HttpResponse(const HttpRequest &Message);
        ~HttpResponse();
        HttpResponse &operator=(HttpResponse const &s);
        HttpResponse(HttpResponse const &src);

        HTTPStatusCode GetStatusCode() const;
        std::string GetResponseBody() const;
        std::string HTTPStatusCodeToString() const;
        std::string GetHttpStatusMessage() const;
    
    

};

#include <sstream>

std::string HttpResponse::GetHttpStatusMessage() const 
{
    switch(this->StatusCode) 
    {
        case HTTP_OK:
            return "OK";
        case HTTP_CREATED:
            return "Created";
        case HTTP_NO_CONTENT:
            return "No Content";
        case HTTP_BAD_REQUEST:
            return "Bad Request";
        case HTTP_UNAUTHORIZED:
            return "Unauthorized";
        case HTTP_FORBIDDEN:
            return "Forbidden";
        case HTTP_NOT_FOUND:
            return "Not Found";
        case HTTP_METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case HTTP_INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
        default:
            return "Unknown";
    }
}

std::string HttpResponse::HTTPStatusCodeToString() const 
{
    std::ostringstream oss;
    oss << this->StatusCode;
    return oss.str();
}

int CheckIfFileExists(std::string FilePath)
{
    std::string tmp = "." + FilePath;
    if (access(tmp.c_str(), F_OK) != -1) 
        return 1;
    return 0;
}

std::string GetResource(const HttpRequest &Request)
{
    if(Request.GetHttpMethod() == "GET")
    {
        int fd;
        std::string FileToOpen = "." + Request.GetPath();
        if(CheckIfFileExists(Request.GetPath()))
        {
            if((fd = open(FileToOpen.c_str(), O_RDONLY)) == -1)
                throw std::runtime_error(std::string("server: opening file:"));
            struct stat st;
            stat(FileToOpen.c_str(), &st);
            int FileSize = st.st_size;
            char Resource[FileSize + 1];
            
            memset(Resource, 0, FileSize + 1);
            int r = read(fd, Resource, FileSize);
            if(r == -1)
                throw std::runtime_error(std::string("server: reading file:"));
            return Resource;
        }
        else 
            return "HTTP_NOT_FOUND";
    }
    else if(Request.GetHttpMethod() == "POST")
    {

    }
    else if(Request.GetHttpMethod() == "DELETE")
    {

    }
    return "HTTP_METHOD_NOT_ALLOWED";
}

std::string HttpResponse::GetResponseBody() const
{
    return this->ResponseBody;
}

HTTPStatusCode HttpResponse::GetStatusCode() const
{
    return this->StatusCode;
}

HTTPStatusCode GetHttpStatusCode(const HttpRequest &Request, const HttpResponse &response)
{
    (void)Request;
    if(response.GetResponseBody()== "HTTP_METHOD_NOT_ALLOWED")
        return HTTP_METHOD_NOT_ALLOWED;
    else if(response.GetResponseBody() == "HTTP_NOT_FOUND")
        return HTTP_NOT_FOUND;
    return HTTP_OK;
}

HttpResponse::HttpResponse(const HttpRequest &Request) : HttpMessage(Request.GetRecivedLine())
{
    this->ResponseBody = GetResource(Request);
    this->StatusCode = GetHttpStatusCode(Request, *this);
}

HttpResponse::~HttpResponse()
{

}

HttpResponse &HttpResponse::operator=(const HttpResponse &s) 
{
    if(this != &s)
    {
    }
    return *this;
}

HttpResponse::HttpResponse(const HttpResponse& copy) 
{
    *this = copy;
}


