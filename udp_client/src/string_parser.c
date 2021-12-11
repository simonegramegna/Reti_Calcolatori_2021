#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include "string_parser.h"

// ascii codes of '+' and '-'
#define PLUS 43
#define MINUS 45

// checks if the inputs are valid
int check_limits(int num)
{
    int check;
    check = 0;

    // checks if the numbers is in the range of integers
    if (num > 0 && num > INT_MAX - num)
    {
        check = 1;
    }
    else
    {
        if (num < 0 && num < INT_MIN - num)
        {
            check = 1;
        }
    }
    return check;
}

// checks if string contains only numbers
int contain_only_numbers(char *str)
{
    int i;
    int contain_numbers;

    i = 0;
    contain_numbers = 1;

    while (str[i] != '\0' && contain_numbers == 1)
    {
        if (!isdigit(str[i]))
        {
            // only '+' and '-' are allowed
            if ((str[i] != MINUS && str[i] != PLUS))
            {
                contain_numbers = 0;
            }

            if ((str[i] == MINUS || str[i] == PLUS) && (i > 0))
            {
                contain_numbers = 0;
            }
        }
        i++;
    }

    return contain_numbers;
}

// checks if the integer is valid
int is_valid_integer(char *str)
{
    int valid;
    valid = 1;

    if (contain_only_numbers(str) == 1)
    {
        int str_num;

        // converts string to number
        str_num = atoi(str);

        if (check_limits(str_num) == 1)
        {
            valid = 0;
        }
    }
    else
    {
        valid = 0;
    }
    return valid;
}

// checks  if the operation is correct
int correct_operation(char operator)
{
    int correct;
    correct = 0;

    // allowed operators
    if (operator== '=' || operator== 'x' || operator== '+' || operator== '/' || operator== '-')
    {
        correct = 1;
    }
    return correct;
}

int valid_input(char *user_input)
{
    int valid;
    char *input_token;
    int token_counter;

    valid = 1;
    token_counter = 0;

    // tokenizes the string
    input_token = strtok(user_input, " ");

    while (input_token != NULL && valid == 1)
    {
        token_counter = token_counter + 1;

        if (token_counter == 1)
        {
            if (correct_operation(input_token[0]) == 0)
            {
                valid = 0;
            }
        }
        else
        {
            valid = is_valid_integer(input_token);
        }
        input_token = strtok(NULL, " ");
    }

    // only one or three tokens are allowed
    if (token_counter == 0 || token_counter == 2 || token_counter > 3)
    {
        valid = 0;
    }
    return valid;
}

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
