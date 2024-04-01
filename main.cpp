/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybourais <ybourais@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:12:14 by ybourais          #+#    #+#             */
/*   Updated: 2024/04/01 17:15:16 by ybourais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h> // socket, bind, listen
#include <unistd.h>

#define SA struct sockaddr_in
#define PORT 80
#define MAXLEN 256

int main() 
{
    //open socket to recive connection
    int ServerFd = socket(PF_INET, SOCK_STREAM, 0);
    if (ServerFd == -1) 
    {
        std::cerr << "ERROR SOCKET: " << strerror(errno)<<std::endl;
        return 1;
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
        socklen_t len = sizeof(address);
        // extracts the first connection request on the queue of pending connections for the listening socket
        int fdconnection = accept(ServerFd, (struct sockaddr *)NULL, NULL);
        if(fdconnection < 0)
        {
            std::cerr << "ERROR ACCEPT: " << strerror(errno)<<std::endl;
            return 1;
        }
        int r = 0;
        char RecivedRequest[MAXLEN] = {0};
        while((r = read(fdconnection, RecivedRequest, MAXLEN - 1)) > 0)
        {
            std::cout<< RecivedRequest<<std::endl;
            if(RecivedRequest[MAXLEN - 1] == '\n')
                break;
            /* memset(message, 0, MESSAGE); */
            /* std::cout << "hello"<<std::endl; */
        }
        if(r < 0)
        {
            std::cerr<< "read ERROR"<<std::endl;
            return 1;
        }
        char mesg[256] = "HTTP/1.0 200 OK\r\n\r\nHI MOM";
        write(fdconnection, &mesg, strlen(mesg));
        close(fdconnection);
    }
    close(ServerFd);
    return 0;
}
