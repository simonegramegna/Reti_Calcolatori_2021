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

#define DEFAULT_ADDR "127.0.0.1"
#define DEFAULT_PORT 27015
#define QLEN 5

//clear the cache

void clearwinsock()
{
#ifdef WIN32
    WSACleanup();
#endif
}
/*
void convert_message(math_message *msg_received)
{

    msg_received->operation = ntohl(msg_received->operation);
    msg_received->n1 = nthol(msg_received->n1);
    msg_received->n2 = nthol(msg_received->n2);
}
*/

/*
char convert_float_to_string(float f){
	 float x=f;

	 char buf[50];

	 gcvt(x, 6, buf);

return buf;
  }
 */
int main(int argc, char **argv)
{
    int server_address;
    int server_port;
    int server_socket;
    struct sockaddr_in localaddress;
    int bind_socket;
    int listen_connection;

//Initialize Winsock

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

    memset(&localaddress, 0, sizeof(localaddress)); // ensures that extra bytes contain 0
    localaddress.sin_family = AF_INET;
    localaddress.sin_addr.s_addr = inet_addr(server_address);
    localaddress.sin_port = htons(server_port);  /* converts values between the host and
													network byte order. Specifically, htons() converts 16-bit quantities
													from host byte order to network byte order. */

    bind_socket = bind(server_socket, (struct sockaddr *)&localaddress,
                       sizeof(localaddress));

    if (bind_socket < 0)
    {
        printf("bind() failed. \n");
        clearwinsock();
        getchar();
        return -1;
    }

//listening socket setting

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

    struct sockaddr_in client_address; // structure for the client address
    int client_socket; // socket descriptor for the client
    int client_size; // the size of the client address

    for (;;)
    {
        client_size = sizeof(client_address); // set the size of the client address
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
            information_rcvd = recv(client_socket, (math_message *)&message_rcvd, message_size, 0);
            //convert_message(&message_rcvd);

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
            result_sent = send(client_socket, (float *)&computed_value, result_size, 0);

            if (result_sent < 0)
            {
                break;
            }
        }
    }
    clearwinsock();
    closesocket(server_socket); //closes the server socket
    getchar();

    return 0;
}
