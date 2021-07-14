#include <iostream>
#include <string.h>
#include "model/Client.h"
#include "model/Server.h"

int main(int argc, char *argv[])
{
    if (argc == 3 && (strcmp(argv[1], "server") == 0))
    {
        Server server{argv[2]};
        server.startServer();
    }
    else if (argc == 4 && (strcmp(argv[1], "client") == 0))
    {
        Client client{argv[2], argv[3]};
        client.startClient();
    }
    else
    {
        std::cout << "ERROR: no es posible ejecutar el juego.\n";
    }

    return 0;
}