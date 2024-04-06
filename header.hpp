/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 20:35:47 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/05 23:11:18 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ostream>
#include <list>
#define MAXLEN 8024

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
        std::string RecivedLine[MAXLEN];
    public:
        HttpRequest();
        HttpRequest(std::string RecivedLine);
        ~HttpRequest();
        HttpRequest &operator=(HttpRequest const &s);//operator overload
        HttpRequest(HttpRequest const &src);     
        
        const std::string HttpMethod() const;
        const std::string Path() const;
        const std::string HttpVersion() const;

};
