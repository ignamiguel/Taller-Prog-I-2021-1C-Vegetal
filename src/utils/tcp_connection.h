#pragma once

#include <sys/types.h>

namespace utils
{
    class TCPConnection
    {
    public:
        TCPConnection(int fd) : sockfd(fd) {};
        ~TCPConnection();
        ssize_t send(const void *buf, size_t len);
        ssize_t recv(void *buf, size_t len);
    private:
        int sockfd;
    };
}