/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:25:59 by ybourais          #+#    #+#             */
/*   Updated: 2024/03/19 16:10:02 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h> // for IPv4 address-related constants and structures
#include <unistd.h>

int main()
{
    // Creating socket file descriptor
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        std::cerr<< "failed to open socket"<<std::endl;
        return 1;
    }
    
    struct sockaddr_in address;
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET; // indicates the type of address the socket will use
    address.sin_port = htons(80800);
    address.sin_addr.s_addr = INADDR_ANY; // any availabe address

    // Forcefully attaching socket to the port 8080
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        std::cerr<< "failed to bind socket to port"<<std::endl;
        return 1;
    }

    // puts the server socket in a passive mode
    if(listen(server_fd, 3) == -1)
    {
        std::cerr<< "failed to puts the server socket in a passive mode"<<std::endl;
        return 1;
    }
    // extracts the first connection request on the queue of pending connections for the listening socket 
    socklen_t addr_length = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_length);   

    char buff[1024] = {0};
    ssize_t r = read(new_socket, buff, 1024 - 1);
    if(r == -1)
        std::cerr<< "failed to read bits"<<std::endl;

    std::cout << "client: " <<buff<<std::endl;

    const char *messege = "hello from server";
    send(new_socket, messege, strlen(messege), 0);

    // closing the connected socket
    close(new_socket);
    
    // closing the listening socket
    close(server_fd);
}


