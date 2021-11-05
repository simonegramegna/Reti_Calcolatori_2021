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
#include "number_operations.h"
/**funzioni di servizio**/

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

int main(int argc, char **argv)
{

	/** inserisco in input la porta utilizzata dalla socket del server **/
	int port = PROTOPORT;
//Inizializzo winsock
#ifdef WIN32
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != NO_ERROR)
	{
		errorhandler("Error at WSAStartup()\n");
		system("pause");
		return 0;
	}
#endif
	//CREAZIONE DELLA SOCKET SERVER
	int socketserver; //descrittore della socket
	socketserver = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketserver < 0)
	{
		errorhandler("socket creation failed. \n");
		clearwinsock();
		system("pause");
		return -1;
	}
	//ASSEGNAZIONE DI UN INDIRIZZO AL SERVER (IP LOCALE + PORTA) ALLA SOCKET SERVER
	struct sockaddr_in localaddress;
	memset(&localaddress, 0, sizeof(localaddress));
	localaddress.sin_family = AF_INET;
	localaddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	localaddress.sin_port = htons(port);
	/**
	 converts values between the host and
	 network byte order. Specifically, htons () converts 16 bit quantities
	 from host byte order to network byte order .
	 **/
	//LEGO LA STRUTTURA INDIRIZZO DEL SERVER AL DESCRITTORE DELLA SOCKET SERVER
	if (bind(socketserver, (struct sockaddr *)&localaddress,
			 sizeof(localaddress)) < 0)
	{
		errorhandler("bind() failed. \n");
		clearwinsock();
		system("pause");
		return -1;
	}
	//SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	if (listen(socketserver, QLEN) < 0)
	{
		errorhandler("listen() failed.\n");
		closesocket(socketserver);
		clearwinsock();
		system("pause");
		return -1;
	}

	struct sockaddr_in client_addr;
	int client_socket;
	int client_size;

	while(TRUE){

		client_size = sizeof(client_addr); //set the size of the client adress
				/*************** (2)Stabilita la connessione, il server invia al client la stringa "connessione avvenuta".**********/

				if ((client_socket = accept(socketserver,
						(struct sockaddr*) &client_addr, &client_size)) < 0) {
					errorhandler("accept() failed.\n");
					//CHIUSURA DELLA CONNESSIONE
					closesocket(client_socket);
					clearwinsock();
					system("pause");
					return 0;
				}
				printf("Connection established with: %s\n", inet_ntoa(client_addr.sin_addr));

				int bytes_rcvd;
				message_operation client_operation;

				char opeartion;
				int op_1;
				int op_2;

	}


	return 0;
}
