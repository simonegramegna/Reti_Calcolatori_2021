#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32

#include <winsock.h>
#else

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unstd.h>
#define closesocket close

#endif
int main(void) {
	puts("porco dio"); /* prints porco dio */
	return EXIT_SUCCESS;
}
