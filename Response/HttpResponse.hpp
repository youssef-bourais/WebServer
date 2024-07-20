/* ************************************************************************** */ /*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:36 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/04 15:43:27 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Request/RequestParsser.hpp"
#include "../parsing/parsingStruct.hpp"

#include <cstdio>
#include <ostream>
#include <string>
#include <sys/syslimits.h>
#include <signal.h>
#include <list>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>

enum HTTPStatusCode 
{
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_BAD_REQUEST = 400,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_URI_TOO_LONG = 414,
    HTTP_ENTITY_TOO_LARGE = 413,
    HTTP_NOT_IMPLEMENTED = 501,
    HTTP_FORBIDDEN,
    HTTP_CONFLICT = 409,
    HTTP_NO_CONTENT = 204,
    HTTP_MOVED_PERMANETLY = 300
};

struct KeyValue 
{
    std::string HttpHeader;
    std::string HttpValue;
    KeyValue(const std::string &k, const std::string &v) : HttpHeader(k), HttpValue(v) {}
};

class HttpResponse
{
    private:
        HTTPStatusCode StatusCode;
        std::string ResponseBody;
        std::list<KeyValue> ResponseHeaders;
        t_servers ServerSetting;
        RequestParsser &Request;
        int Confignum;

    public:
        HttpResponse(RequestParsser &Request, t_servers &ServerSetting);
        ~HttpResponse();
        HttpResponse &operator=(HttpResponse const &s);
        HttpResponse(HttpResponse const &src);

        HTTPStatusCode GetStatusCode() const;
        std::string GetResponseBody() const;
        std::string HTTPStatusCodeToString() const;
        std::string GetHttpStatusMessage() const;
        void SetHTTPStatusCode(HTTPStatusCode Code);

        void SetConfigNum(int index);
        
        std::list<KeyValue>::const_iterator GetHeadersBegin() const;
        std::list<KeyValue>::const_iterator GetHeadersEnd() const;
};




