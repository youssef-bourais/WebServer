/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/02 22:18:10 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <iostream>
#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h> // socket, bind, listen
#include <unistd.h>

#define SA struct sockaddr_in
#define PORT 80
#define MAXLEN 4096

int main() 
{
    //open socket to recive connection
    int ServerFd = socket(PF_INET, SOCK_STREAM, 0);
    if (ServerFd == -1) 
    {
        std::cerr << "ERROR SOCKET: " << strerror(errno)<<std::endl;
        return 1;
    }   
    int opt = 1;
    // it helps in reuse of address and port. Prevents error such as: “address already in use”
    if (setsockopt(ServerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    SA address;
    memset(&address, 0,sizeof(address));
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY); // we use htonl to convert the INADDR_ANY constant to network byte order
    address.sin_port = htons(PORT);
    
    //assigns the address specified by addr to the socket referred to by the file descriptor serverfd
    if(bind(ServerFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "ERROR BIND: " << strerror(errno)<<std::endl;
        return 1;
    }
    // listen put the server in the passivemode 3 is maximum length to which the queue of pending connections for sockfd may grow
    if(listen(ServerFd, 3) < 0)
    {
        std::cerr << "ERROR LISTEN: " << strerror(errno)<<std::endl;
        return 1;
    }
    while(true) 
    {
        std::cout << "waiting for conection on Port: " << PORT <<std::endl;
        // extracts the first connection request on the queue of pending connections for the listening socket
        int fdconnection = accept(ServerFd, (struct sockaddr *)NULL, NULL);
        if(fdconnection < 0)
        {
            std::cerr << "ERROR ACCEPT: " << strerror(errno)<<std::endl;
            return 1;
        }
        int r = 0;
        char RecivedRequest[MAXLEN + 1] = {0};
        memset(RecivedRequest, 0, MAXLEN);
        std::string msg;
        r = 1;
        /* while(true) */
        /* { */
            r = read(fdconnection, RecivedRequest, MAXLEN);
            printf("%d\n", r);
            std::cout<< RecivedRequest<<std::endl;
            /* if(RecivedRequest[MAXLEN - 1] == '\n' || r == 0) */
            /* { */
                /* break; */
            /* } */
            memset(RecivedRequest, 0, MAXLEN);
        /* } */
        std::cout << "readed"<<std::endl;
        if(r < 0)
        {
            std::cerr<< "read ERROR"<<std::endl;
            return 1;
        }
        // Send response to the client
        const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5\r\n\r\nhello";
        int response_length = strlen(response);
        if (send(fdconnection, response, response_length, 0) != response_length) 
        {
            std::cerr << "Send error: " << strerror(errno) << std::endl;
            close(fdconnection);
            return 1;
        }
        /* write(fdconnection, &response, strlen(response)); */
        /* send(fdconnection, response, strlen(response), 0); */
        close(fdconnection);
    }
    close(ServerFd);
    return 0;
}

