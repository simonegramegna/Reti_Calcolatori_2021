#ifndef MATH_MESSAGE_H_
#define MATH_MESSAGE_H_

#define HOST_NAME_DIM 50

// data type that defines data to send to the server
typedef struct
{
    char host_name[HOST_NAME_DIM];
    char operation;
    int n1;
    int n2;

} math_message;

#endif
