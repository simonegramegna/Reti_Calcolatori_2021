#include <stdio.h>
#include <stdlib.h>
#include "math_message.h"

#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#define NO_ERROR 0
#endif

// default port and address
#define DEFAULT_ADDR "127.0.0.1"
#define DEFAULT_PORT 27015

// maximum length of the user input string
#define DIM_INPUT 30

//librerie standard
//#define BUFFERSIZE 512
//#define PROTOPORT 27015

//clear Windows cache
void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char **argv[])
{

/*
 * All Winsock applications must be initialized to make sure that the
 * sockets windows are supported by the system. 
 * To initialize Winsock, an element of type WSADATA must be created.
*/
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

//CREAZIONE DELLA CLIENT SOCKET
	int clientsocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientsocket < 0) {
		printf("Socket creation failed.\n");
		closesocket(clientsocket);
		clearwinsock();
		system("pause");
		return -1;
	}
//COSTRUZIONE DELL'INDIRIZZO DEL SERVER DA CUI IL CLIENT DEVE INVIARE MESSAGGI
	struct sockaddr_in localaddress;
	memset(&localaddress, 0, sizeof(localaddress));
	localaddress.sin_family = AF_INET;
	localaddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	localaddress.sin_port = htons("127.0.0.1");

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
		int send=sendto(clientsocket,(math_message*)&requested_computation, (int) sizeof(math_message), 0, (struct sockaddr*)&localaddress, sizeof localaddress);

		if (send_operation < 0)
		{
			printf("send() operation failed.\n");
			closesocket(clientsocket);
			clearwinsock();
			getchar();
			return -1;
		}


	    // server response
		result_size = (int)sizeof(float);
		server_response = recv(clientsocket, (float *)&result_rcvd, result_size, 0);

		if (server_response <= 0)
		{
			printf("recv() failed.\n");
			closesocket(clientsocket);
			clearwinsock();
			getchar();
			return -1;
		}
        // displays the result of the computation
		printf("\n%d %c %d = %.2f\n", requested_computation.n1, requested_computation.operation, requested_computation.n2, result_rcvd);
	}

//main end
	closesocket(clientsocket);
	clearwinsock();
	system("pause");
	return 0;
} // main end

