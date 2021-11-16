#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <math.h>
#include <sstream>
#include <string>
using namespace std;

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER; //Allow for mutual exclusion to handle critical sections
int randomNum, divisor = 2; //Initialize global varibles for determing if random number is prime

int main()
{
	srand(time(NULL));
	int client, server;
	int portNum = 1488;
	bool isExit = false;
	int bufsize = 1044;
	char buffer[bufsize];

	struct sockaddr_in server_addr,client_addr;
	memset(&server_addr, '0', sizeof(server_addr)); //Initialize socket struct
	socklen_t size;

	/* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
	/* --------------- socket() function ------------------*/

	client = socket(AF_INET, SOCK_STREAM, 0);

	if (client < 0)
	{
		cout << "\nError establishing socket..." << endl;
		exit(1);
	}

	cout << "\n=> Socket server has been created..." << endl;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr= inet_addr("127.0.0.1"); //Set interface for server to listen on
	server_addr.sin_port = htons(portNum);

	/* ---------- BINDING THE SOCKET ---------- */
	/* ---------------- bind() ---------------- */


	bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
		cout << "=> Error binding connection, the socket has already been established..." << endl;
		//return -1;
	

	size = sizeof(server_addr);
	cout << "=> Looking for clients..." << endl;

	/* ------------- LISTENING CALL ------------- */
	/* ---------------- listen() ---------------- */

	listen(client, 10);
	int s = sizeof(struct sockaddr_in);
	
	int k=0;
	int numberPower;
	int clientCount = 1;
int cfd = 0; //Initialize variable for sending numbers to clients

	// first check if it is valid or not
	if (server < 0)
		cout << "=> Error on accepting..." << endl;
	//strcpy(buffer, "=> Server connected...\n");
		//send(server, buffer, bufsize, 0);
		//server = accept(client, (struct sockaddr *)&server_addr, &size);
		cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
		cout << "\n=> Enter # to end the connection\n" << endl;


		
			cout << "\nServer: ";
		
				//int parameter, checkPrime = 0; //Initialize parameter for lfd input
				//memcpy(&parameter, (int *)lfdInput, sizeof(int)); //Store input lfd in parameter
				
				while (1)
	{server = accept(client, (struct sockaddr *)&server_addr, (socklen_t*)&s);
				int n = rand() % 100000; //Generate random, large number
				//cfd = accept(n, (struct sockaddr*)NULL, NULL);
				int base = rand()%9;
				while (base < 2) {
					base = rand() % 9;
				}
				int temp=n;
				int result = 0;
				//char snum[5];
				// convert 123 to string [buf]
				//itoa(n, snum, 10);
				string asd = to_string(n);
				int power=0;
				int a=asd.length();
				//The calculation of the conversion into base 10
				for (int i=a-1;i>=0;i--) {
				 int number= temp%10;
				 result =result+(number*pow(base,power));
				temp=temp/10;
				power++;
				}

				char sBuffer[1044], rBuffer[10]; //Output buffer char array
				memset(sBuffer, '3', sizeof(sBuffer)); //Initialize output buffer
				memset(rBuffer, '0', sizeof(rBuffer)); //Initialize output buffer
				bool array[result];
				for (int i = 0; i<result; i++) {
					if (i == 0 || i == 1) {
						array[i] = false;
					}
					else {
					array[i] = true;
					}
				}
				for (int i = 2; i < result; i++) {
					if (i % 2 == 0 && i != 2) {
						array[i] = false;
					}
				}


				for (int i = 3; i <= sqrt(result); i += 2) {
					for (int j = i; j < result ; j += i) {
						if (!array[i] || j%i == 0 && i != j) {
							array[j] = false;
						}
					}
				}
				int j=2;
				int number[int(sqrt(result))];
				number[0]=result;
				number[1]=base;
				for (int i = 0; i<result; i++) {
					if (array[i] == true) {
						number[j]=i;	
						j++;			
					}
				}
				string arrays[100];
				int count=0;
				int counter=0;
				stringstream ss;
		
		
				while(sBuffer[k]!='0'&&sBuffer!="0"&&number[k]<=result+1&&number[k]!=0) {
				int randomNumber=number[k];
				snprintf(sBuffer, sizeof(sBuffer) , "%d", randomNumber); //Send random number to output buffer
				if(k==0) {
					cout<<"number in base is "<<randomNumber<<endl;
					
				}
				if(k==1) {
					cout<<"the base number is "<<randomNumber<<endl;
				}
				if(randomNumber==result&&k!=0) {
					cout<<"the number is a prime number"<<endl;
					close(server);
					exit(0);
					}
				send(server, sBuffer, sizeof(sBuffer),MSG_NOSIGNAL);
				bzero(sBuffer, 1044); //Clear output buffer
				
				while(read(server, sBuffer, sizeof(sBuffer))==0) ;
					
				numberPower=atoi(sBuffer);
				bzero(sBuffer, 1044); //Clear output buffer
				
				if(numberPower!=0) {
					ss << randomNumber<<" ^ "<<numberPower<<"* ";
					arrays[counter]= ss.str();
					counter++;
					cout<<endl;
					cout<< number[0]<< "("<<number[1]<<") ="<< ss.str();
					cout<<endl;
				//close(server);			
				}
		k++;	
		
		}
		//close(server);
		//close(client);
		//close(cfd);
	}

	close(client);
	return 0;
}
