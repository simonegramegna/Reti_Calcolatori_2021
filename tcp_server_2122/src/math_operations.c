#include <stdio.h>
#include <float.h>

#include "math_operations.h"

int add(int a, int b){

	return a+b;
}

int diff(int a, int b){

	return a-b;
}

int mult(int a, int b){

	return a*b;
}

float division(int a, int b){

	if(b==0){

		return FLT_MAX;
	}else{

		return (float) a/b;
	}
}
