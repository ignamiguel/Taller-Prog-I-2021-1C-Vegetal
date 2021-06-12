#include "tcp_server.h"

#include <stdexcept>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// TODO: Get server port from the configuration file? 
#define PORT 6666

namespace utils
{
    TcpServer::TcpServer()
    {
        socket_server = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_server == -1)
        {
            throw std::runtime_error("ERROR: Unable to create server socket");
        }
        struct sockaddr_in servaddr = { 0 };

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(PORT);

        if (!bind(socket_server, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
        {
            throw std::runtime_error("ERROR: Unable to bind server socket");
        }
    }

    void TcpServer::start(void)
    {
        // Create a new thread for this (producer)
        int socket_client = -1;

        if (listen(socket_server, 5) == -1)
        {
            throw std::runtime_error("ERROR: Unable to listen server socket");
        }
        
        struct sockaddr_in servaddr = { 0 };
        socklen_t len = sizeof(servaddr);
  
        while (true)
        {
            // Get client
            socket_client = accept(socket_server, (struct sockaddr *)&servaddr, &len);

            // Process client in a new thread (consumer)
        }
    }

    void TcpServer::stop(void)
    {
        // set event to stop producer and costumer threads
        close(socket_server);
    }
}