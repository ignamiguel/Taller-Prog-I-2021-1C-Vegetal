#include "tcp_connection.h"

#include <unistd.h>
#include <sys/socket.h>

namespace utils
{
    TCPConnection::TCPConnection(int fd) : sockfd(fd)
    {
    }

    TCPConnection::~TCPConnection()
    {
        ::close(sockfd);
    }

    ssize_t TCPConnection::send(const void *buf, size_t len)
    {
        return ::send(sockfd, buf, len, 0);
    }

    ssize_t TCPConnection::recv(void *buf, size_t len)
    {
        // Wait until there is data to read?
        return ::recv(sockfd, buf, len, 0);
    }
}