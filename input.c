#include <stdio.h>

int main()
{

    int v1, v2;
    char a;

    do
    {
        printf("\n\ninserisci a v1 v2\n");
        scanf("%c %d %d",&a,&v1,&v2);

        printf("a: %c v1: %d v2: %d ", a, v1, v2);
    } while (a != '=');
    return 0;
}