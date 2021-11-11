#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define BUFFERSIZE 512	// defualt buffer size
#define QLEN 6			// length of queue of waiting clients

// function to print error messages
void error_handler(char *error_message)
{
	printf("%s", error_message);
}

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
		error_handler("Error at WSAStartup()\n");
		system("pause");
		return 0;
	}
#endif

	server_port = PROTOPORT;
	server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_socket < 0)
	{
		error_handler("socket creation failed. \n");
		clearwinsock();
		system("pause");
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
		error_handler("bind() failed. \n");
		clearwinsock();
		system("pause");
		return -1;
	}

	listen_connection = listen(server_socket, QLEN);

	if (listen_connection < 0)
	{
		error_handler("listen() failed.\n");
		closesocket(server_socket);
		clearwinsock();
		system("pause");
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

			error_handler("accept() failed.\n");
			closesocket(client_socket);
			clearwinsock();
			system("pause");
			return -1;
		}

		printf("Connection established with: %s\n", inet_ntoa(client_address.sin_addr));
		//int information_rcvd;
		//int message_size;
	}

	return 0;
}
