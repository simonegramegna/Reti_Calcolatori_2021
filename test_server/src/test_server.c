#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 27015
#define MAX_CONN 6
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
int main(void) {

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

	int serverSocket = socket(PF_INET,SOCK_STREAM, IPPROTO_TCP );

	if( serverSocket < 0){
		printf("errrore nella creazione della socket \n");
		return 0;
	}

	struct sockaddr_in addrServer;
	memset(&addrServer, 0, sizeof(addrServer));
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_port = htons(SERVER_PORT);

	int bindServer = bind(serverSocket, (struct sockaddr*) &addrServer, sizeof(addrServer));

	if(bindServer < 0){
		printf("errore nel collegamento tra socket e indirizzo\n");
			printf("codice: %d",bindServer);
		return 0;
	}

	// mi metto in ascolto
	int listenServer = listen(serverSocket, MAX_CONN);

	if(listenServer < 0){
		printf("errore nell'ascolto del server");
		return 0;
	}

	// accetto connessioni dal client
			struct sockaddr_in clientAddr;
			int clientSocket;
			int clientLen;

	while(1){


		clientLen = sizeof(clientAddr);

		clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddr, &clientLen);

		if(clientSocket < 0){
			printf("errore nell'accept \n");
			closesocket(serverSocket);
			return 0;
		}

		char* buf;

		int recvData = recv(clientSocket, buf,5, 0 );
		printf("received data %s", buf);

	}

	closesocket(serverSocket);



#if defined WIN32
	WSACleanup();
#endif



	return 0;
}
