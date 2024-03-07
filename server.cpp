
#include <sys/socket.h>
#include <sys/_types.h> // for data types used in socket-related functions
#include <iostream>
#include <netinet/in.h> // for IPv4 address-related constants and structures

int main()
{
    int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        std::cerr<< "failed to open socket"<<std::endl;
        return 1;
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET; // indicates the type of address the socket will use
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY; // any availabe address
 
    if(bind(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        std::cerr<< "failed to bind socket to port"<<std::endl;
        return 1;
    }
    if(listen(sockfd, 3) == -1)
    {
        std::cerr<< "failed to puts the server socket in a passive mode"<<std::endl;
        return 1;
    }
    socklen_t addr_length = sizeof(address);
    int new_socket = accept(sockfd, (struct sockaddr *)&address, &addr_length);   


    std::cout << sockfd<<std::endl;
}



