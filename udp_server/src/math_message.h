#ifndef MATH_MESSAGE_H_
#define MATH_MESSAGE_H_

// data type that defines data to send to the server
typedef struct
{
    char operation;
    int n1;
    int n2;

} math_message;

#endif
