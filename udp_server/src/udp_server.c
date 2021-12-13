#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math_message.h"
#include "string_parser.h"
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
#define LEN_HOSTNAME 50
#define RESULT_DIGITS 10
#define DEFAULT_PORT 27015
#define DEFAULT_SERVER "calculator.uniba.it"

void clearwinsock()
{
#if defined WIN32
    WSACleanup();
#endif
}

int main(int argc, char **argv)
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

    int port;
    int bind_socket;
    int server_socket;
    parsed_addr input_addr;
    struct hostent *server_name;
    struct in_addr *ip_server_addr;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    char server[HOSTNAME_LEN] = DEFAULT_SERVER;
    port = DEFAULT_PORT;

    if (argc == 2)
    {
        get_addr_port(&input_addr, argv[1]);
        strcpy(server, input_addr.host_name);
        port = input_addr.port;

        if (port < 0 || port > 65535)
        {
            port = DEFAULT_PORT;
        }
    }

    server_name = gethostbyname(server);
    ip_server_addr = (struct in_addr *)server_name->h_addr_list[0];

    // creates server socket
    server_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (server_socket < 0)
    {
        printf("Socket server creation failed.\n");
        closesocket(server_socket);
        clearwinsock();
        getchar();
        return -1;
    }

    // memory allocation for server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*ip_server_addr));
    server_addr.sin_port = htons(port);

    bind_socket = bind(server_socket, (struct sockaddr *)&server_addr,
                       sizeof(server_addr));

    if (bind_socket < 0)
    {
        printf("Bind failed.\n");
        closesocket(server_socket);
        clearwinsock();
        getchar();
        return -1;
    }
    printf("Waiting for a client message...\n");

    for (;;)
    {
        int client_addr_len;
        int information_rcvd;
        math_message math_operation;
        char operation_rcvd[DIM_INPUT];

        client_addr_len = sizeof(client_addr);
        information_rcvd = recvfrom(server_socket, operation_rcvd, DIM_INPUT, 0, (struct sockaddr *)&client_addr, &client_addr_len);

        if (information_rcvd <= 0)
        {
            printf("recvfrom() failed.\n");
            break;
        }

        math_operation = get_math_message(operation_rcvd);

        int n1;
        int n2;
        char operation;
        float computed_value;
        struct hostent *client_name;

        n1 = math_operation.n1;
        n2 = math_operation.n2;
        operation = math_operation.operation;

        client_name = gethostbyaddr((char *)&client_addr.sin_addr, LEN_HOSTNAME, AF_INET);
        printf("Requested operation: %c %d %d  from client: %s, IP: %s\n", operation, n1, n2, client_name->h_name, inet_ntoa(client_addr.sin_addr));

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
        char computed_value_str[DIM_INPUT];

        // converts float to string
        gcvt(computed_value, RESULT_DIGITS, computed_value_str);

        // sends data back to client
        result_sent = sendto(server_socket, computed_value_str, DIM_INPUT, 0, (struct sockaddr *)&client_addr, client_addr_len);

        if (result_sent != DIM_INPUT)
        {
            printf("sendto() failed.\n");
            break;
        }
    }

    closesocket(server_socket);
    clearwinsock();
    getchar();
    return 0;
}
