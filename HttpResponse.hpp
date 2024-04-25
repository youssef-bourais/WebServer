/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:36 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/24 19:12:44 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpMessage.hpp"
#include "HttpRequest.hpp"

/*response headers Server, Date, Content-Length, and Content-Type, Allow*/

enum HttpStatus_Code 
{
    OK = 200,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500
};

class HttpResponse : public HttpMessage
{
    private:
        std::string StatusCode;
        std::string Body;
    public:
        HttpResponse(const HttpRequest &Message);
        ~HttpResponse();
        HttpResponse &operator=(HttpResponse const &s);
        HttpResponse(HttpResponse const &src);

        std::string GetStatusCode() const;
        std::string GetBody() const;
};

int getStatusCode(const HttpRequest &Request)
{
    (void)Request;
    return OK;
}


std::string HttpResponse::GetStatusCode() const
{
    return this->StatusCode;
}

HttpResponse::HttpResponse(const HttpRequest &Request)
{
    this->StatusCode = getStatusCode(Request);
    this->Body = "";
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


