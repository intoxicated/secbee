#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char test = '\x55';
    short t2 = 0x5556;
    printf("0x%x \n", test);
    printf("0x%x \n", t2);
    return 0;
}
