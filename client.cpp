/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:25:52 by ybourais          #+#    #+#             */
/*   Updated: 2024/03/19 16:23:57 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h> //     struct sockaddr_in serv_addr;
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

 
int main()
{
    // Creating socket file descriptor
    int client_fd;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        std::cerr<< "failed to open socket"<<std::endl;
        return 1;
    }
 
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
 
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        std::cerr<<"Invalid address/ Address not supported \n";
        return 1;
    }
 
    int status; 
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    const char *message = "Hello from client";
    send(client_fd, message, strlen(message), 0);
    std::cout << "message send from server."<<std::endl;
    char buffer[1024] = {0};
    int valread; 
    valread = read(client_fd, buffer, 1024 - 1); // subtract 1 for the null terminator at the end
    printf("%s\n", buffer);
 
    // closing the connected socket
    close(client_fd);
    return 0;
}



