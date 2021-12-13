#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_parser.h"

math_message get_math_message(char *user_input)
{
    math_message requested_operation;
    int token_counter;
    char *input_token;

    token_counter = 0;

    // tokenizes the string
    input_token = strtok(user_input, " ");

    while (input_token != NULL)
    {
        token_counter = token_counter + 1;

        // first token is the operator
        if (token_counter == 1)
        {
            requested_operation.operation = input_token[0];
        }

        // the second and third token are numbers
        if (token_counter == 2)
        {
            requested_operation.n1 = atoi(input_token);
        }
        if (token_counter == 3)
        {
            requested_operation.n2 = atoi(input_token);
        }
        input_token = strtok(NULL, " ");
    }
    return requested_operation;
}

void get_addr_port(parsed_addr *host_addr_port, char *server_name_port)
{
    int i;
    i = 0;

    while (i < strlen(server_name_port))
    {

        if (server_name_port[i] == ':')
        {
            host_addr_port->host_name[i] = '\0';
            i++;
            break;
        }

        host_addr_port->host_name[i] = server_name_port[i];
        i++;
    }

    char port_str[HOSTNAME_LEN];
    int j;
    j = 0;

    while (i < strlen(server_name_port))
    {
        port_str[j] = server_name_port[i];
        i++;
        j++;
    }
    port_str[j] = '\0';
    host_addr_port->port = atoi(port_str);
}
