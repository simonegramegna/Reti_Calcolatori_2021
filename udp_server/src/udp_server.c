#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math_message.h"
#include "math_operations.h"

#ifdef WIN32
#include <winsock.h>
#else

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close

#endif

#define DIM_INPUT 30

void clearwinsock()
{
#if defined WIN32
    WSACleanup();
#endif
}

int main()
{
#ifdef WIN32
    // Initialize Winsock
    WSADATA wsa_data;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (iResult != 0)
    {
        printf("Error at WSAStartup().\n");
        getchar();
        return -1;
    }
#endif

    int server_socket;
    int bind_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (server_socket < 0)
    {
        printf("socket server creation failed.\n");
        closesocket(server_socket);
        clearwinsock();
        getchar();
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(27015);

    bind_socket = bind(server_socket, (struct sockaddr *)&server_addr,
                       sizeof(server_addr));

    if (bind_socket < 0)
    {

        printf("bind failed.\n");
        closesocket(server_socket);
        clearwinsock();
        getchar();
        return -1;
    }
    printf("waiting for a client message...\n");
    
    for (;;)
    {
        int client_addr_len;
        int information_rcvd;
        int message_size;
        //math_message message_rcvd;
        char operation_rcvd[DIM_INPUT];

        client_addr_len = sizeof(client_addr);
        //message_size = (int)sizeof(message_rcvd);

        information_rcvd = recvfrom(server_socket, operation_rcvd, DIM_INPUT, 0, (struct sockaddr *)&client_addr, &client_addr_len);

        if (information_rcvd <= 0)
        {
            printf("recvfrom server failed.\n");
            break;
        }

        printf("connection established with: %s\n",
               inet_ntoa(client_addr.sin_addr));

        printf("Operazione ricevuta: %s\n", operation_rcvd);
        /*
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
        }*/
        /*
        
        int result_size;

        result_size = (int)sizeof(float);*/
        int result_sent;
        result_sent = sendto(server_socket, operation_rcvd, DIM_INPUT, 0, (struct sockaddr *)&client_addr, client_addr_len);

        if (result_sent != DIM_INPUT)
        {

            break;
        }
    }

    closesocket(server_socket);
    clearwinsock();
    getchar();

    return 0;
}
