#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32

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

#endif

int main() {

#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result =
		WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0)
	{
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif


	int clientSocket = socket(PF_INET,SOCK_STREAM, IPPROTO_TCP );

	if( clientSocket < 0){
		printf("errrore nella creazione della socket \n");
		return 0;
	}

	struct sockaddr_in clientAddr;
	memset(&clientAddr,0, sizeof(clientAddr) );
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr.sin_port = htons(27015);

	// mi connetto al server
	int conn = connect(clientSocket, (struct sockaddr*) &clientAddr, sizeof(clientAddr));

	if(conn < 0){

		printf("errore nella connessione al server\n");
		return 0;
	}

	char* str = "dome";
	int sendData = send(clientSocket, str, sizeof(str), 0);

	if(sendData != strlen(str)){
		printf("errore nell'invio: inviati meno dati");

	}




	return 0;
}
