#include "../C1222_ACSE.h"
#include "../C1222_EPSEM.h"

#include <string>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

int main (int argc, char ** argv)
{
    C1222_ACSE acse;

    uint8_t  data[] = "\x60\x29\xA2\x05\x80\x03\x7B\xC1\x75\
\xA6\x04\x80\x02\x7B\x04\xA8\x03\x02\x01\x07\
\xBE\x15\x28\x13\x81\x11\x80\x0F\x50\x00\x02\
\x55\x53\x45\x52\x20\x4E\x41\x4D\x45\x20\x00\x3C";
    acse.parse(data);

    C1222_EPSEM epsem;

    epsem.parse(acse.get_epsem());


    return 0;
}
