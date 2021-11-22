#ifndef STRING_PARSER_H_
#define _STRING_PARSER_H_

#include "math_message.h"

// checks if the user input is valid
int valid_input(char *user_input);

// gets math_message from user input
math_message get_math_message(char *user_input);

#endif
