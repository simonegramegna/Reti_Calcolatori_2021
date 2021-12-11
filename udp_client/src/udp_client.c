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
#define DEFAULT_SERVER "calculator.uniba.it"

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

    int port = DEFAULT_PORT;
    char server[HOSTNAME_LEN] = DEFAULT_SERVER;
    parsed_addr input_addr;

    struct hostent *remote_server;

    if (argc == 2)
    {
        get_addr_port(&input_addr, argv[1]);

        //printf("Name %s, port %d", input_addr.host_name, input_addr.port);
        strcpy(server, input_addr.host_name);
        port = input_addr.port;
    }

    //CREAZIONE DELLA CLIENT SOCKET
    int client_socket;

    client_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket < 0)
    {
        printf("socket client creation failed.\n");
        closesocket(client_socket);
        clearwinsock();
        getchar();
        return -1;
    }

    remote_server = gethostbyname(server);

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)remote_server->h_addr)));
    client_addr.sin_port = htons(port);

    math_message requested_computation;
    int operation_size;
    int send_operation;
    char user_input[DIM_INPUT];
    char parsed_user_input[DIM_INPUT];

    for (;;)
    {
        float result_rcvd;
        int result_size;
        int server_response;

        // gets user input
        printf("Enter the operation in this format: operator[+,-,x,\\], first number[integer], second number[integer], press = to quit\n");
        gets(user_input);
        strcpy(parsed_user_input, user_input);

        // repeats while the input is not valid
        while (valid_input(user_input) == 0)
        {
            printf("Input NOT valid, please enter again the operation in this format: operator[+,-,x,\\], first number[integer], second number[integer], press = to quit\n");
            gets(user_input);
            strcpy(parsed_user_input, user_input);
        }

        // quit if '=' is pressed
        if (user_input[0] == '=')
        {
            printf("Exiting...\n");
            break;
        }

        // gets data type to send from the correct input string
        requested_computation = get_math_message(parsed_user_input);
        operation_size = (int)sizeof(requested_computation);

        // sends the data type to the server
        send_operation = sendto(client_socket, &requested_computation, (int)operation_size, 0, (struct sockaddr *)&client_addr, sizeof client_addr);

        if (send_operation < 0)
        {
            printf("send() operation failed.\n");
            closesocket(client_socket);
            clearwinsock();
            getchar();
            return -1;
        }

        // server response
        result_size = (int)sizeof(float);
        server_response = recvfrom(client_socket, &result_rcvd, result_size, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

        if (server_response <= 0)
        {
            printf("recv() failed.\n");
            closesocket(client_socket);
            clearwinsock();
            getchar();
            return -1;
        }
        // displays the result of the computation
        printf("\n%d %c %d = %.2f\n", requested_computation.n1, requested_computation.operation, requested_computation.n2, result_rcvd);
    }

    //main end
    closesocket(client_socket);
    clearwinsock();
    getchar();

    return 0;
}
