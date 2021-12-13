#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    const char *name = "calculator.uniba.it";
    struct hostent *lh = gethostbyname(name);

    if (lh)
    {
        struct in_addr *ina = (struct in_addr *)lh->h_addr_list[0];
        printf("Risultato di gethostbyname(%s): %s\n", name,inet_ntoa(*ina));
        struct sockaddr_in *aa;
        

        for (int i = 0; i < 2; i++)
        {
            printf("elem %s\n", lh->h_addr_list[i]);
        }
    }
    else
    {
        herror("gethostbyname");
    }

    return 0;
}