#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_parser.h"

int correct_operation(char operator)
{
    int correct;
    correct = 0;

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
            int str_num;
            str_num = atoi(input_token);

            if (str_num == 0 && strlen(input_token) != 1 && input_token[0] != '0')
            {
                valid = 0;
            }
        }
        input_token = strtok(NULL, " ");
    }

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
    input_token = strtok(user_input, " ");

    while (input_token != NULL)
    {
        token_counter = token_counter + 1;

        if (token_counter == 1)
        {
            requested_operation.operation = input_token[0];
        }

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