#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "math_message.h"
#include "string_parser.h"

// socket libraries
#ifdef WIN32

#include <winsock.h>
#else

#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

void clearwinsock()
{
#ifdef WIN32
    WSACleanup();
#endif
}

int main(int argc, char **argv)
{
#ifdef WIN32

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        printf("Error at WSAStartup().\n");
        system("pause");
        return -1;
    }
#endif

    int client_socket;
    struct sockaddr_in server_address;

    client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (client_socket < 0)
    {
        printf("Socket creation failed.\n");
        closesocket(client_socket);
        clearwinsock();
        getchar();
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(27015);

    int server_connection;
    server_connection = connect(client_socket, (struct sockaddr *)&server_address,
                                (int)sizeof(server_address));

    if (server_connection < 0)
    {
        printf("Failed to connect.\n");
        closesocket(client_socket);
        clearwinsock();
        getchar();
        return -1;
    }

    math_message requested_computation;
    int operation_size;
    int send_operation;
    char user_input[30];
    char parsed_user_input[30];

    do
    {
        printf("Enter the operation in this format: operator[+,-,x,\], first number[integer], second number[integer], press = to quit\n");
        gets(user_input);
        strcpy(parsed_user_input, user_input);

        while (valid_input(user_input) == 0)
        {

            printf("Input NOT valid, please enter again the operation in this format: operator[+,-,x,\], first number[integer], second number[integer], press = to quit\n");
            gets(user_input);
            strcpy(parsed_user_input, user_input);
        }

        if (stop_command(parsed_user_input) == 1)
        {
            printf("Exiting...\n");
            break;
        }
        requested_computation = get_math_message(parsed_user_input);
        operation_size = (int)sizeof(requested_computation);
        send_operation = send(client_socket, (math_message *)&requested_computation, operation_size, 0);

        if (send_operation < 0)
        {
            printf("send() operation failed.\n");
            closesocket(client_socket);
            clearwinsock();
            getchar();
            return -1;
        }

        float result_rcvd;
        int result_size;
        int server_response;

        result_size = (int)sizeof(float);
        server_response = recv(client_socket, (float *)&result_rcvd, result_size, 0);

        if (server_response <= 0)
        {
            printf("recv() failed.\n");
            closesocket(client_socket);
            clearwinsock();
            getchar();
            return -1;
        }
        printf("\n%d %c %d = %.2f\n",  requested_computation.n1,  requested_computation.operation,  requested_computation.n2, result_rcvd);

    } while (1);

    printf("Close connection.\n");
    closesocket(client_socket);
    clearwinsock();
    getchar();

    return 0;
}
