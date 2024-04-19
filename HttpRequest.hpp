/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 20:38:06 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/19 17:15:30 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <list>
#include <ostream>
#define MAXLEN 8192

/*parssing the request*/
/* HTTP method, requested URL, headers, and request body */

struct KeyValue 
{
    std::string HttpHeader;
    std::string HttpValue;
    KeyValue(const std::string &k, const std::string &v) : HttpHeader(k), HttpValue(v) {}
};

class HttpRequest
{
    private:
        std::list<KeyValue> HttpHeaders;
        std::string RecivedLine;
    public:
        HttpRequest();
        HttpRequest(std::string RecivedLine);
        ~HttpRequest();
        HttpRequest &operator=(const HttpRequest &s);
        HttpRequest(const HttpRequest &copy);
        
        void PrintHeaders() const;
        void PrintRequest() const;
        const std::string HttpMethod() const;
        const std::string Path() const;
        const std::string HttpVersion() const;

};

