#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math_operations.h"
#include "math_message.h"

// socket libraries
#ifdef WIN32

#include <winsock.h>
#else

#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define PROTOPORT 27015 // defualt protocol number
#define BUFFERSIZE 512  // defualt buffer size
#define QLEN 6          // length of queue of waiting clients

void clearwinsock()
{
#ifdef WIN32
    WSACleanup();
#endif
}

int main(int arcg, char **argv)
{
    int server_port;
    int server_socket;
    struct sockaddr_in localaddress;
    int bind_socket;
    int listen_connection;

#ifdef WIN32

    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (result != 0)
    {
        printf("Error at WSAStartup()\n");
        getch();
        return 0;
    }
#endif

    server_port = PROTOPORT;
    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket < 0)
    {
        printf("socket creation failed. \n");
        clearwinsock();
        getch();
        return -1;
    }

    memset(&localaddress, 0, sizeof(localaddress));
    localaddress.sin_family = AF_INET;
    localaddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    localaddress.sin_port = htons(server_port);

    bind_socket = bind(server_socket, (struct sockaddr *)&localaddress,
                       sizeof(localaddress));

    if (bind_socket < 0)
    {
        printf("bind() failed. \n");
        clearwinsock();
        getch();
        return -1;
    }

    listen_connection = listen(server_socket, QLEN);

    if (listen_connection < 0)
    {
        printf("listen() failed.\n");
        closesocket(server_socket);
        clearwinsock();
        getch();
        return -1;
    }

    struct sockaddr_in client_address;
    int client_socket;
    int client_size;

    for (;;)
    {
        client_size = sizeof(client_address);
        client_socket = accept(server_socket,
                               (struct sockaddr *)&client_address, &client_size);

        if (client_socket < 0)
        {

            printf("accept() failed.\n");
            closesocket(client_socket);
            clearwinsock();
            getch();
            return -1;
        }

        printf("Connection established with: %s\n", inet_ntoa(client_address.sin_addr));

        int information_rcvd;
        int message_size;
        math_message message_rcvd;

        message_size = (int)sizeof(message_rcvd);
        information_rcvd = recv(client_socket, (math_message *)&message_rcvd, message_size, 0);

        if (information_rcvd <= 0)
        {

            printf("recv() string failed.\n");
            closesocket(server_socket);
            clearwinsock();
            getch();
            return -1;
        }

        int op1;
        int op2;
        float computed_value;
        char operation;

        op1 = message_rcvd.operator_1;
        op2 = message_rcvd.operator_2;
        operation = message_rcvd.operation;

        switch (operation)
        {
        case '+':
            computed_value = (float)add(op1, op2);
            break;

        case '-':
            computed_value = (float)diff(op1, op2);
            break;

        case 'x':

            computed_value = (float)mult(op1, op2);
            break;

        case '/':
            computed_value = division(op1, op2);
            break;
        default:
            continue;
        }

        int result_sent;
        int result_size;

        result_size = (int)sizeof(float);
        result_sent = send(client_socket, (float *)&computed_value, result_size, 0);

        if (result_sent < 0)
        {

            printf("send() stringhe modificate failed.\n");
            closesocket(result_sent);
            clearwinsock();
            getch();
            return -1;
        }
    }
    clearwinsock();
    closesocket(server_socket);
    getch();

    return 0;
}
