/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:55 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/19 17:53:20 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

HttpResponse::HttpResponse(HttpRequest &Request)
{
    this->Request = Request;
    this->Request.PrintHeaders();
}

HttpResponse::~HttpResponse()
{

}

HttpResponse &HttpResponse::operator=(const HttpResponse &s) 
{
    if(this != &s)
    {
        this->Request = s.Request;
    }
    return *this;
}

HttpResponse::HttpResponse(const HttpResponse& copy) 
{
    *this = copy;
}


