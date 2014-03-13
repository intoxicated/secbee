#include "../C1222_ACSE.h"
#include "../C1222_EPSEM.h"
#include "../C1222_Request.h"

#include <string>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

int main (int argc, char ** argv)
{

    uint8_t  data[] = "\x60\x29\xA2\x05\x80\x03\x7B\xC1\x75\
\xA6\x04\x80\x02\x7B\x04\xA8\x03\x02\x01\x07\
\xBE\x15\x28\x13\x81\x11\x80\x0F\x50\x00\x02\
\x55\x53\x45\x52\x20\x4E\x41\x4D\x45\x20\x00\x3C";

    uint8_t data2[] = "\x60\x22\xa2\x04\x80\x02\x7b\x04\
\xa4\x03\xa6\x05\x80\x03\x7b\xc1\x75\xa8\x03\x02\x01\x07\
\xbe\x09\x28\x07\x81\x05\x80\x03\x00\x00\x3c";

    C1222_Request_Logon logon (2, "USER NAME", 60);
    uint8_t * d = logon.build(); // build data;
    uint8_t * logonData = d;
    printf("    [*] request data length : %lx\n", logon.get_build_size());
    for(int i = 0; i < logon.get_build_size(); ++i)
        printf("0x%02x ", d[i]);
    puts("");
    C1222_EPSEM epsem (d, 0x80, 0, logon.get_build_size());
    d = epsem.build();
    for(int i = 0; i < epsem.get_build_size(); ++i)
        printf("0x%02x ", d[i]);
    puts("");
    printf("    [*] epsem length : %lx\n", epsem.get_build_size());
    puts("");

    C1222_ACSE acse ( d, "123.4", "7", "123.8437", NULL, epsem.get_build_size());
    d = acse.build();       

    puts("");
    printf("[!] FINAL\n");
    for(int i = 0; i < acse.get_data_len(); ++i)
        printf("0x%02x ", d[i]);

    puts("");

    C1222_Request_Logon * p = 
        (C1222_Request_Logon*)C1222_Request_Parser::parse(logonData);

    std::cout << "User id: " << p->get_user_id() << endl;
    std::cout << "Timeout: " << p->get_timeout() << endl;
    std::cout << "Username " << p->get_username() << endl;
    
    delete p;

    return 0;
}
