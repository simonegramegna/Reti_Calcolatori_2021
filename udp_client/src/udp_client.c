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
    /*
    parsed_addr input_addr;

    struct hostent *remote_server;

    if (argc == 2)
    {
        get_addr_port(&input_addr, argv[1]);

        //printf("Name %s, port %d", input_addr.host_name, input_addr.port);
        strcpy(server, input_addr.host_name);
        port = input_addr.port;
    }*/

    //CREAZIONE DELLA CLIENT SOCKET
    int client_socket;
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;

    client_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (client_socket < 0)
    {
        printf("socket client creation failed.\n");
        closesocket(client_socket);
        clearwinsock();
        getchar();
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(port);

    math_message requested_computation;
    int operation_size;
    int send_operation;
    char user_input[DIM_INPUT];
    char parsed_user_input[DIM_INPUT];

    for (;;)
    {
        //float result_rcvd;
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
        /*
        // gets data type to send from the correct input string
        requested_computation = get_math_message(parsed_user_input);
        operation_size = (int)sizeof(requested_computation);

        strcpy(requested_computation.host_name , server);*/
        // sends the data type to the server
        send_operation = sendto(client_socket, parsed_user_input, DIM_INPUT, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (send_operation < 0)
        {
            printf("send() operation failed.\n");
            closesocket(client_socket);
            clearwinsock();
            getchar();
            return -1;
        }

        char result[DIM_INPUT];

        // server response
        //result_size = (int)sizeof(float);
        int server_len = sizeof(server_addr);
        server_response = recvfrom(client_socket, result, DIM_INPUT, 0, (struct sockaddr *)&server_addr, &server_len);

        if (server_response <= 0)
        {
            printf("recv() failed.\n");
            closesocket(client_socket);
            clearwinsock();
            getchar();
            return -1;
        }
        printf("\nRisposta del server %s", result);
        // displays the result of the computation
        //printf("\n%d %c %d = %.2f\n", requested_computation.n1, requested_computation.operation, requested_computation.n2, result_rcvd);
    }

    //main end
    closesocket(client_socket);
    clearwinsock();
    getchar();

    return 0;
}
