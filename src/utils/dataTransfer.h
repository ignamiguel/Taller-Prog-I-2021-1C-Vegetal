#pragma once
#include <sys/socket.h>

template<typename T>
size_t sendData(const int clientSocket, const T *const data)
{
    size_t totalBytesSent = 0;
    ssize_t bytesSent = 0;
    auto dataSize = sizeof(T);

    while (totalBytesSent < dataSize)
    {
        bytesSent = send(clientSocket, (data + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
        if (bytesSent > 0)
        {
            totalBytesSent += bytesSent;
        }
        else
        {
            break;
        }
    }
    return totalBytesSent;
}

template<typename T>
size_t receiveData(const int clientSocket, T *const data)
{
    size_t totalBytesReceived = 0;
    ssize_t bytesReceived = 0;
    auto dataSize = sizeof(T);

    while (totalBytesReceived < dataSize)
    {
        bytesReceived = recv(clientSocket, (data + totalBytesReceived), (dataSize - totalBytesReceived), MSG_NOSIGNAL);
        if (bytesReceived > 0)
        {
            totalBytesReceived += bytesReceived;
        }
        else
        {
            break;
        }
    }

    return totalBytesReceived;
}