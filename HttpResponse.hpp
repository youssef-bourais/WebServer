/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:36 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/04 15:27:57 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpRequest.hpp"
#include <cstring>
#include <list>
#include <sys/fcntl.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <sys/stat.h>

#include <sstream>
#include "HttpMessage.hpp"

enum HTTPStatusCode 
{
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_BAD_REQUEST = 400,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_INTERNAL_SERVER_ERROR = 500
};

class HttpResponse : public HttpMessage
{
    private:
        HTTPStatusCode StatusCode;
        std::string ResponseBody;
        std::list<KeyValue> ResponseHeaders;
    public:
        HttpResponse(const HttpRequest &Message);
        ~HttpResponse();
        HttpResponse &operator=(HttpResponse const &s);
        HttpResponse(HttpResponse const &src);

        HTTPStatusCode GetStatusCode() const;
        std::string GetResponseBody() const;
        std::string HTTPStatusCodeToString() const;
        std::string GetHttpStatusMessage() const;
        void SetHTTPStatusCode(HTTPStatusCode Code);
        
        std::list<KeyValue>::const_iterator GetHeadersBegin() const;
        std::list<KeyValue>::const_iterator GetHeadersEnd() const;

};

std::string GetDayName(int n) 
{
    switch(n) 
    {
        case 0:
            return "Sunday";
        case 1:
            return "Monday";
        case 2:
            return "Tuesday";
        case 3:
            return "Wednesday";
        case 4:
            return "Thursday";
        case 5:
            return "Friday";
        case 6:
            return "Saturday";
        default:
            return "Invalid day of week";
    }
}


std::string GetMonthName(int n) 
{
    switch(n) 
    {
        case 0:
            return "January";
        case 1:
            return "February";
        case 2:
            return "March";
        case 3:
            return "April";
        case 4:
            return "May";
        case 5:
            return "June";
        case 6:
            return "July";
        case 7:
            return "August";
        case 8:
            return "September";
        case 9:
            return "October";
        case 10:
            return "November";
        case 11:
            return "December";
        default:
            return "Invalid month";
    }
}

std::string HttpResponse::GetHttpStatusMessage() const 
{ 
    switch(this->StatusCode) 
    {
        case HTTP_OK:
            return "OK";
        case HTTP_CREATED:
            return "Created";
        case HTTP_BAD_REQUEST:
            return "Bad Request";
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




