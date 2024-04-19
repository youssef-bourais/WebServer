/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:39:36 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/19 17:14:35 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpRequest.hpp"


class HttpResponse
{
    private:
        HttpRequest Request;
    public:
        HttpResponse(HttpRequest &Request);
        ~HttpResponse();
        HttpResponse &operator=(HttpResponse const &s);
        HttpResponse(HttpResponse const &src);
};


