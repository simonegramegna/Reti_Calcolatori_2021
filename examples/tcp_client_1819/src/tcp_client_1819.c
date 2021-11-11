//Librerie per le socket
#ifdef WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
//Librerie standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PROTOPORT 27015 //defualt protocol number
#define BUFFERSIZE 512	//defualt buffer size
#define QLEN 6
#define TRUE 1

#include "message.h"
#include "parse_input.h"

//Funzione che stampa come messaggio di errore la stringa data in input
void errorhandler(char *errorMessage)
{
	printf("%s", errorMessage);
}
//Funzione che pulisce la struttura WSADATA utilizzata per inizializzare le socket in windows
void clearwinsock()
{
#ifdef WIN32
	WSACleanup();
#endif
}

int main()
{

//INIZIALIZZO WINSOCK
#ifdef WIN32
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		errorhandler("Error at WSAStartup().\n");
		system("pause");
		return -1;
	}
#endif
	//CREAZIONE DELLA SOCKET CLIENT
	int socketclient; //descrittore della socket client
	socketclient = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketclient < 0)
	{
		printf("Socket creation failed.\n");
		closesocket(socketclient);
		clearwinsock();
		//getc();
		return -1;
	}

	//CREAZIONE DI UN INDIRIZZO SERVER (IP LOCALE + PORTA) A CUI IL CLIENT SI DEVE CONNETTERE
	struct sockaddr_in serveraddress;
	memset(&serveraddress, 0, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddress.sin_port = htons(27015);
	/**converts values between the host and
			 network byte order. Specifically, htons () converts 16 bit quantities
			 from host byte order to network byte order .
			 **/

	/****************(1) Il client richiede la connessione al server.********************************/

	//CONNESSIONE AL SERVER
	int server_connection;
	server_connection = connect(socketclient, (struct sockaddr *)&serveraddress,
								(int)sizeof(serveraddress));

	if (server_connection < 0)
	{
		errorhandler("Failed to connect.\n");
		closesocket(socketclient);
		clearwinsock();
		//getc();
		return -1;
	}

	// se la connessione e' andata bene
	message_operation requested_operation;
	int bytes_sent;
	int send_operation;

	printf("Inserisci il primo numero: \n");
	scanf("%d", &requested_operation.operator_1);
	printf("\nInserisci il secondo numero: \n");
	scanf("%d", &requested_operation.operator_2);
	printf("Inserisci l'operazione da eseguire: ");
	scanf(" %c", &requested_operation.operation);

	bytes_sent = (int)sizeof(requested_operation);
	send_operation = send(socketclient, (message_operation *)&requested_operation, bytes_sent, 0);

	if (send_operation < 0)
	{
		errorhandler("send() string failed.\n");
		closesocket(socketclient);
		clearwinsock();
		//getc();
		return -1;
	}

	printf("\n\n");
	int result;
	int result_size;
	int server_response;

	result_size = (int)sizeof(int);
	server_response = recv(socketclient, (int *)&result, result_size, 0);

	if (server_response <= 0)
	{
		errorhandler("recv() STRINGHE MODIFICATE failed.\n");
		closesocket(socketclient);
		clearwinsock();
		//getc();
		return -1;
	}

	printf("Il risultato e' %d ", result);

	char *s1;
	s1 = malloc(256 * sizeof(char));
	printf("Inserisci la stringa di operazione \n");
	scanf("%s", s1);

	printf(" \n risultato %d", valid_input(s1));

	printf("Close connection.\n");
	closesocket(socketclient);
	clearwinsock();
	//getc();

	return 0;
}
