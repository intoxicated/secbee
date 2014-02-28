#include "Utils.hpp"
#include <iostream>
#include <stdlib.h>
#include <string>
#include "C1222.h"

/**
 * TODO: Test util functions 
 *
 */

int main()
{
    char * ret;
    int size;
   
    ret = ber_uid_decode((void *)"\x03\x7B\xC1\x75", &size); 

    char  input[] = "156.5454";
    ap_element * enc = ber_uid_encode(input, 8);
    printf("return value is :%s\n", ret);

    printf("encoded %s is:\n", input);
    printf("totalsize: %d\n", enc->size);
    for(int i = 0; i < enc->size; i++)
    {
        printf("%02x:", enc->data[i]);
    }
    puts("");
    if(enc != NULL){
        free(enc->data);
        free(enc);
    }
    if(ret != NULL)
        free(ret);
    return 0;
}

