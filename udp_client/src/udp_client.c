#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math_message.h"
#include "string_parser.h"

#if defined WIN32
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
#define DEFAULT_PORT 27015
#define DEFAULT_SERVER "localhost"

void clearwinsock()
{
#if defined WIN32
    WSACleanup();
#endif
}

int main(int argc, char **argv)
{

#if defined WIN32

    // Initialize Winsock
    WSADATA wsa_data;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (iResult != 0)
    {
        printf("Error at WSAStartup().\n");
        system("pause");
        return -1;
    }
#endif

    int port;
    int client_socket;
    parsed_addr input_addr;
    struct in_addr *in_addr_server;
    struct sockaddr_in server_addr;
    struct hostent *remote_server_name;

    char server[HOSTNAME_LEN] = DEFAULT_SERVER;
    port = DEFAULT_PORT;

    if (argc == 2)
    {
        get_addr_port(&input_addr, argv[1]);
        strcpy(server, input_addr.host_name);
        port = input_addr.port;

        // port check
        if (port < 0 || port > 65535)
        {
            port = DEFAULT_PORT;
        }
    }

    // gets ip address of server from name
    remote_server_name = gethostbyname(server);
    in_addr_server = (struct in_addr *)remote_server_name->h_addr_list[0];

    // creation of client socket
    client_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (client_socket < 0)
    {
        printf("Socket client creation failed.\n");
        closesocket(client_socket);
        clearwinsock();
        getchar();
        return -1;
    }

    // memory allocation for server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*in_addr_server));
    server_addr.sin_port = htons(port);

    int send_operation;
    char user_input[DIM_INPUT];
    char parsed_user_input[DIM_INPUT];

    for (;;)
    {
        int server_len;
        int server_response;
        char result[DIM_INPUT];

        server_len = sizeof(server_addr);

        // gets user input
        printf("Enter the operation in this format: operator[+,-,x,\\]   first number[integer]   second number[integer] press = to quit\n");
        gets(user_input);
        strcpy(parsed_user_input, user_input);

        // repeats while the input is not valid
        while (valid_input(user_input) == 0)
        {
            printf("Input NOT valid, please enter again the operation in this format: operator[+,-,x,\\]   first number[integer]   second number[integer] press = to quit\n");
            gets(user_input);
            strcpy(parsed_user_input, user_input);
        }

        // quit if '=' is pressed
        if (user_input[0] == '=')
        {
            printf("Exiting...\n");
            break;
        }

        // sends the data type to the server
        send_operation = sendto(client_socket, parsed_user_input, DIM_INPUT, 0, (struct sockaddr *)&server_addr, server_len);

        if (send_operation < 0)
        {
            printf("sendto() failed.\n");
            closesocket(client_socket);
            clearwinsock();
            getchar();
            return -1;
        }

        // gets response string from server
        server_response = recvfrom(client_socket, result, DIM_INPUT, 0, (struct sockaddr *)&server_addr, &server_len);

        if (server_response <= 0)
        {
            printf("recvfrom() failed.\n");
            closesocket(client_socket);
            clearwinsock();
            getchar();
            return -1;
        }

        double num_result;
        math_message printed_message;

        // gets numbers and operator to be printed on screen
        printed_message = get_math_message(parsed_user_input);

        // converts string result into double
        num_result = atof(result);

        // displays the result of the computation
        printf("\nResult got from server: %s, IP: %s : %d %c %d = %.2lf\n\n", server, inet_ntoa(*in_addr_server), printed_message.n1, printed_message.operation, printed_message.n2, num_result);
    }

    closesocket(client_socket);
    clearwinsock();
    getchar();
    return 0;
}
