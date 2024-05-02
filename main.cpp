/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/05/02 16:02:06 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "HttpServer.hpp"
#include "HttpMessage.hpp"
#include "HttpRequest.hpp"


int main() 
{
    try 
    {
        HttpServer Server;
        Server.ForceReuse();
        Server.BindSocketToAddr();
        Server.StartListining(3);
        while(1)
        {
            Server.AccepteConnectionAndRecive();
            HttpRequest Request(Server.GetRequest());
            Request.PrintRequest();
            HttpResponse Response(Request);

            Server.SendResponse(Response);
        }
    } 
    catch (std::runtime_error& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

/**/
/* #include <stdio.h> */
/* #include <dirent.h> */
/**/
/* int main()  */
/* { */
/*     DIR *dir; */
/*     struct dirent *entry; */
/**/
/*     dir = opendir("./minipage/"); */
/*     if (dir == NULL)  */
/*     { */
/*         perror("opendir"); */
/*         return 1; */
/*     } */
/*     while ((entry = readdir(dir)) != NULL)  */
/*     { */
/*         printf("%s\n", entry->d_name); */
/*     } */
/**/
/*     closedir(dir); */
/*     return 0; */
/* } */



