#pragma once

#include <memory>

#include "safe_queue.h"
#include "tcp_connection.h"

namespace utils
{
    class TcpServer
    {
        public:
            TcpServer();
            ~TcpServer();
            void start(void);
            void stop(void);

        private:
            int socket_server;
            utils::SafeQueue<std::unique_ptr<utils::TCPConnection>> clients;
    };
}
