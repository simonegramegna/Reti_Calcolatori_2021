#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math_operations.h"
#include "math_message.h"

#ifdef WIN32
// ms-windows socket library
#include <winsock.h>
#else

#define closesocket close
// unix/Linux socket libraries
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

// default port and address
#define DEFAULT_ADDR "127.0.0.1"
#define DEFAULT_PORT 27015

// queue length of clients
#define QLEN 5

//clear the cache
void clearwinsock()
{
#ifdef WIN32
    WSACleanup();
#endif
}

int main(int argc, char **argv)
{
    int server_address;
    int server_port;
    int server_socket;
    struct sockaddr_in localaddress;
    int bind_socket;
    int listen_connection;

/*
* All Winsock applications must be initialized to make sure that the
* sockets windows are supported by the system.
* To initialize Winsock, an element of type WSADATA must be created.
*/
#ifdef WIN32

    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (result != 0)
    {
        printf("Error at WSAStartup()\n");
        getchar();
        return 0;
    }
#endif

    server_address = DEFAULT_ADDR;
    server_port = DEFAULT_PORT;
    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // gets address and port to connect from command line if there are two arguments
    if (argc == 2)
    {
        server_address = argv[0];
        server_port = atoi(argv[1]);

        if (server_port < 0 || server_port > 65535)
        {
            server_port = DEFAULT_PORT;
        }
    }

    if (server_socket < 0)
    {
        printf("socket creation failed. \n");
        clearwinsock();
        getchar();
        return -1;
    }

    memset(&localaddress, 0, sizeof(localaddress)); 
    localaddress.sin_family = AF_INET;
    localaddress.sin_addr.s_addr = inet_addr(server_address);
    localaddress.sin_port = htons(server_port); 

    // binds socket to the address
    bind_socket = bind(server_socket, (struct sockaddr *)&localaddress,
                       sizeof(localaddress));

    if (bind_socket < 0)
    {
        printf("bind() failed. \n");
        clearwinsock();
        getchar();
        return -1;
    }

    // listens incoming connections
    listen_connection = listen(server_socket, QLEN);

    if (listen_connection < 0)
    {
        printf("listen() failed.\n");
        closesocket(server_socket);
        clearwinsock();
        getchar();
        return -1;
    }

    //accept a new connection
    struct sockaddr_in client_address; 
    int client_socket;                 
    int client_size;                   

    for (;;)
    {
        // set the size of the client address
        client_size = sizeof(client_address);

        // accepts the connection from client
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_size);
        
        if (client_socket < 0)
        {
            continue;
        }

        printf("Connection established with: %s\n", inet_ntoa(client_address.sin_addr));

        for (;;)
        {
            int information_rcvd;
            int message_size;
            math_message message_rcvd;

            message_size = (int)sizeof(message_rcvd);

            // gets data from client
            information_rcvd = recv(client_socket, (math_message *)&message_rcvd, message_size, 0);

            if (information_rcvd <= 0)
            {
                break;
            }

            int n1;
            int n2;
            float computed_value;
            char operation;

            n1 = message_rcvd.n1;
            n2 = message_rcvd.n2;
            operation = message_rcvd.operation;

            switch (operation)
            {
            case '+':
                computed_value = (float)add(n1, n2);
                break;

            case '-':
                computed_value = (float)diff(n1, n2);
                break;

            case 'x':

                computed_value = (float)mult(n1, n2);
                break;

            case '/':
                computed_value = division(n1, n2);
                break;
            default:
                continue;
            }

            int result_sent;
            int result_size;

            result_size = (int)sizeof(float);

            // sends data to client
            result_sent = send(client_socket, (float *)&computed_value, result_size, 0);

            if (result_sent < 0)
            {
                break;
            }
        }
    }
    clearwinsock();
    closesocket(server_socket); 
    getchar();

    return 0;
}
