#include "Utils.hpp"
#include <iostream>
#include <stdlib.h>
#include <string>
#include "C1222.h"

int main()
{
    char * ret;
    int size;
   
    ret = ber_uid_decode((void *)"\x03\x7B\xC1\x75", &size); 

    char  input[] = "123.8437";
    ap_element * enc = ber_uid_encode(input, 8);
    printf("return value is :%s\n", ret);

    if(enc != NULL){
        free(enc->data);
        free(enc);
    }
    if(ret != NULL)
        free(ret);
    return 0;
}

