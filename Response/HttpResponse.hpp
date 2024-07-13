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

#include "../Request/HttpRequest.hpp"
#include <cstring>
#include <list>
#include <sys/fcntl.h>


#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <sys/stat.h>

#include <sstream>
#include "../Request/HttpMessage.hpp"
#include "../Request/RequestParsser.hpp"

#include "../parsing/parsingStruct.hpp"

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
    HTTP_NO_CONTENT = 204
};

class HttpResponse
{
    private:
        HTTPStatusCode StatusCode;
        std::string ResponseBody;
        std::list<KeyValue> ResponseHeaders;
        t_servers ServerSetting;
        RequestParsser Request;

    public:
        HttpResponse(const RequestParsser &Request, t_servers &ServerSetting);
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




