/*!
* Simple chat program (client side).cpp - http://github.com/hassanyf
* Version - 2.0.1
*
* Copyright (c) 2016 Hassan M. Yousuf
*/

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>

using namespace std;

int main()
{

	srand(time(NULL));
	int client;
	int portNum = 1488; // NOTE that the port number is same for both client and server
	bool isExit = false;
	int bufsize = 1044;
	char buffer[bufsize];
	char *ip = "127.0.0.1";
	int numBase;
	int server = 0;
	socklen_t size;
	int randomPrime;
	struct sockaddr_in server_addr;
	memset(&server_addr, '0', sizeof(server_addr)); //Initialize socket struct
	int num;
	client = socket(AF_INET, SOCK_STREAM, 0);
	size = sizeof(server_addr);
	if (client < 0)
	{
		cout << "\nError establishing socket..." << endl;
		exit(1);
	}

	cout << "\n=> Socket client has been created..." << endl;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Set interface for server to listen on
	/* ---------- CONNECTING THE SOCKET ---------- */
	/* ---------------- connect() ---------------- */

	connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr));
	cout << "=> Connection to the server port number: " << portNum << endl;

	int power = 0;
	cout << "=> Awaiting confirmation from the server..." << endl; //line 40
	//num = read(client, buffer, sizeof(buffer) - 1);
	//printf(buffer, sizeof(buffer), "%d", num);
	//num=atoi(buffer);

	cout << "=> Connection confirmed, you are good to go...";
	cout << "\n\n=> Enter # to end the connection\n"
		 << endl;
	int temp = num;
	int number2 = 0;
	//bzero(buffer, 100); //Clear output buffer
	//numBase = read(client, buffer, sizeof(buffer) - 1);
	//cout<<"base number "<<endl;
	//printf(buffer, sizeof(buffer), "%d", numBase); //Send random number to output buffer
	int readsize;
	int store;
	memset(buffer, 'e', sizeof(buffer)); //Initialize output buffer
	while (1)
	{
		//client = accept(client, (struct sockaddr *)&server_addr, &size);
		randomPrime = read(client, buffer, sizeof(buffer));

		if (number2 == 0)
		{

			randomPrime = atoi(buffer);
			num = randomPrime;
			temp = num;
			number2++;
			cout << "number in base 10 is ";
			printf(buffer, sizeof(buffer), "%d", num);
			cout << endl;
			memset(buffer, 'e', sizeof(buffer)); //Initialize output buffer
		}
		else if (number2 == 1)
		{
			for (int i = 0; i < 1043; i++)
			{
				if (buffer[i] != 'e')
				{
					randomPrime = (int)buffer[i];
					//break;
				}
			}
			numBase = randomPrime;
			number2++;
			cout << "base number is ";
			printf(buffer, 1044, "%d", numBase);
			cout << endl;
			memset(buffer, 'e', sizeof(buffer)); //Initialize output buffer
		}
		else
		{
			string str = "";
			for (int i = 0; i < 1043; i++)
			{
				if (buffer[i] != 'e')
				{
					str += buffer[i];
					//break;
				}
			}
			randomPrime = atoi(str.c_str());
			power = 0;
			cout << endl;
			cout << "random prime is ";
			printf(buffer, sizeof(buffer), "%d", randomPrime);
			//return -1; //Exit program with error
			memset(buffer, 'e', sizeof(buffer)); //Initialize output buffer
			if (randomPrime == 0)
			{
				cout << "error divide by zero" << endl;
				exit(0);
			}
			while (num % randomPrime == 0)
			{
				if (randomPrime != 0)
				{
					num /= randomPrime;
					power++;
					store = num;
				}
			}
			num = temp;
		}
		snprintf(buffer, sizeof(buffer), "%d", power); //Send random number to output buffer
		cout << " and power is " << power;
		if (store != 1)
		{
			send(client, buffer, sizeof(buffer), MSG_NOSIGNAL);
		}
		memset(buffer, 'e', sizeof(buffer)); //Initialize output buffer
											 //sleep(1);

		//accept(client, (struct sockaddr *)&server_addr, &size);
		//close(client);
		//cout << "\n=> Connection terminated.\nGoodbye...\n";
	}

	//close(client);
	return 0;
}
