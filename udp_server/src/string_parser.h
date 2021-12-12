#ifndef STRING_PARSER_H_
#define _STRING_PARSER_H_

#include "math_message.h"
#define HOSTNAME_LEN 50

typedef struct{

    char host_name[HOSTNAME_LEN];
    int port;

} parsed_addr;

// checks if the user input is valid
int valid_input(char *user_input);

// gets math_message from user input
math_message get_math_message(char *user_input);

// gets parsed_addr form input string
void get_addr_port(parsed_addr *host_addr_port, char *server_name_port);

#endif
