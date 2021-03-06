#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "math_message.h"
#include "string_parser.h"

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

// maximum length of the user input string
#define DIM_INPUT 30

// clear the cache
void clearwinsock()
{
#ifdef WIN32
	WSACleanup();
#endif
}

int main(int argc, char **argv)
{

/*
 * All Winsock applications must be initialized to make sure that the
 * sockets windows are supported by the system. 
 * To initialize Winsock, an element of type WSADATA must be created.
*/
#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (iResult != 0) {
		printf("Error at WSAStartup().\n");
		system("pause");
		return -1;
	}
#endif

// creation of the client socket
	int port;
	int address;
	int client_socket;
	struct sockaddr_in server_address;
	client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	port = DEFAULT_PORT;
	address = DEFAULT_ADDR;

	if (client_socket < 0)
	{
		printf("Socket creation failed.\n");
		closesocket(client_socket);
		clearwinsock();
		getchar();
		return -1;
	}

    // gets address and port to connect from command line if there are two arguments
	if (argc == 2)
	{
		address = argv[0];
		port = atoi(argv[1]);

		if (port < 0 || port > 65535)
		{
			port = DEFAULT_PORT;
		}
	}

	// creation of server address
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(address);
	server_address.sin_port = htons(port);

	// connection to the server
	int server_connection;
	server_connection = connect(client_socket, (struct sockaddr *)&server_address, (int)sizeof(server_address));

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
		send_operation = send(client_socket, (math_message *)&requested_computation, operation_size, 0);

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
		server_response = recv(client_socket, (float *)&result_rcvd, result_size, 0);

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

	printf("Close connection.\n");
	closesocket(client_socket);
	clearwinsock();
	getchar();

	return 0;
}
