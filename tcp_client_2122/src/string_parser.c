#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_parser.h"

char get_operator(char* input_string){

    char operator;

    if(strlen(input_string) > 1){

        operator = input_string[0];
    }
    return operator;
}

