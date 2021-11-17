#ifndef STRING_PARSER_H_
#define _STRING_PARSER_H_

#include "math_message.h"

int valid_input(char *input_string);

int stop_command(char *input_string);

math_message get_math_message(char *user_input);

#endif