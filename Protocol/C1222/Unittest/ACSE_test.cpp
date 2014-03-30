#include <gtest/gtest.h>
#include <iostream>

#include "../C1222_Request.h"
#include "../C1222_EPSEM.h"
#include "../C1222_ACSE.h"
#include "../C1222_Response.h"
//TODO
uint8_t data[] = "\x60\x29\xA2\x05\x80\x03\x7B\xC1\x75\
\xA6\x04\x80\x02\x7B\x04\xA8\x03\x02\x01\x07\
\xBE\x15\x28\x13\x81\x11\x80\x0F\x50\x00\x02\
\x55\x53\x45\x52\x20\x4E\x41\x4D\x45\x20\x00\x3C";

uint8_t data2[] = "\x60\x22\xa2\x04\x80\x02\x7b\x04\
\xa4\x03\x02\x01\x07\xa6\x05\x80\x03\x7b\xc1\x75\xa8\x03\x02\x01\x00\
\xbe\x09\x28\x07\x81\x05\x80\x03\x00\x00\x3c";

class ACSE_Build : public ::testing::Test {

};

class ACSE_Parse : public ::testing::Test {

};

//check building down to top a
TEST_F(ACSE_Build, test_build)
{
    C1222_Request_Logon logon (2,"USER NAME", 60);
    uint8_t * d = logon.build(); // build data;
    uint8_t * logonData = d;
    printf("    [*] request data length : %lx\n", logon.get_build_size());

    puts("");
    C1222_EPSEM * epsem = new C1222_EPSEM(d, 0x80, 0, logon.get_build_size());
    d = epsem->build();

    printf("    [*] epsem length : %lx\n", epsem->get_build_size());
    puts("");

    C1222_ACSE * acse = new C1222_ACSE( d, "123.4", "7", "123.8437", NULL, epsem->get_build_size());
    
    d = acse->build();      

    for(int i = 0; i < acse->get_build_size(); ++i)
        EXPECT_EQ(data[i], d[i]);

    delete epsem;
    delete acse;
}


//check building down to top a
TEST_F(ACSE_Build, test_response_build)
{
    C1222_Response_Logon logon (0x0, 60);
    uint8_t * d = logon.build(); // build data;
    uint8_t * logonData = d;

    C1222_EPSEM * epsem = new C1222_EPSEM(d, 0x80, 0, logon.get_build_size());
    d = epsem->build();

    C1222_ACSE * acse = new C1222_ACSE( d, "123.8437", "0", "123.4", "7", epsem->get_build_size());
    
    d = acse->build();      
    printf("ACSE SIZE : %d\n", acse->get_build_size());
    for(int i = 0; i < acse->get_build_size(); i++)
        EXPECT_EQ(data2[i], d[i]);

    delete epsem;
    delete acse;
}
//check parsing top to down
TEST_F(ACSE_Parse, test_parse)
{

	
}
