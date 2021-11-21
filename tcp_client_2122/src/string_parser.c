#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "string_parser.h"

#define PLUS 43
#define MINUS 45

int check_limits(int num)
{
    int check;
    check = 0;

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

int is_valid_integer(char *str)
{
    int valid;
    valid = 1;

    if (contain_only_numbers(str) == 1)
    {
        int str_num;
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
            valid = is_valid_integer(input_token);
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