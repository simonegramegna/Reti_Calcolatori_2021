#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_parser.h"

int valid_input(char *input_string){


    int valid;
    char *input_token;
    int token_counter;

    valid = 1;
    token_counter = 0;
    input_token = strtok(input_string, " ");

    while(input_token != NULL && valid == 1){

        token_counter = token_counter + 1;

        if(token_counter == 1){

            

        }else{

        }
    }

    
}