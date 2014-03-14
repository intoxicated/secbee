#include <gtest/gtest.h>
#include <iostream>

#include "../C1222_Response.h"
#include "../C1222_EPSEM.h"
#include "../C1222_ACSE.h"

//TODO
uint8_t data[] = "\x60\x29\xA2\x05\x80\x03\x7B\xC1\x75\
\xA6\x04\x80\x02\x7B\x04\xA8\x03\x02\x01\x07\
\xBE\x15\x28\x13\x81\x11\x80\x0F\x50\x00\x02\
\x55\x53\x45\x52\x20\x4E\x41\x4D\x45\x20\x00\x3C";

uint8_t data2[] = "\x60\x22\xa2\x04\x80\x02\x7b\x04\
\xa4\x03\xa6\x05\x80\x03\x7b\xc1\x75\xa8\x03\x02\x01\x07\
\xbe\x09\x28\x07\x81\x05\x80\x03\x00\x00\x3c";

class Response_Build : public ::testing::Test {

};

class Response_Parse : public ::testing::Test {

};

TEST_F(Response_Build, test_Ident)
{
	C1222_Response_Ident ident(0x0, 0x3, 0x1, 0x0);
    
    uint8_t * d = ident.build();

    uint8_t res, std, ver, rev;

    memcpy(&res, d, 1);
    memcpy(&std, (d+1), 1);
    memcpy(&ver, (d+2), 1);
    memcpy(&rev, (d+3), 1);

    EXPECT_EQ(0x0, res);
    EXPECT_EQ(0x3, std);
    EXPECT_EQ(0x1, ver);
    EXPECT_EQ(0x0, rev);
}

TEST_F(Response_Build, test_Logon)
{
    C1222_Response_Logon logon(0x0, 0x3C);
    uint8_t * d = logon.build();
    short timeout;
    logon.get_build_size();
    memcpy(&timeout, d+1, 2);
    timeout = ntohs(timeout);

    EXPECT_EQ(0x3C, timeout);
    EXPECT_EQ(0x0, *d);
}

TEST_F(Response_Build, test_Resolve)
{
    C1222_Response_Resolve resolve(0x0, 0x4, 
                (uint8_t*)"\x01\x02\x03\x04");

    uint8_t * d = resolve.build();
    uint8_t addr[5];
    memcpy(addr, d+2, *(d+1));
    addr[*(d+1)] = '\0';

    EXPECT_EQ(0x0, *d);
    EXPECT_EQ(0x4, *(d+1));
    EXPECT_STREQ("\x01\x02\x03\x04", (char *)addr); 
}

TEST_F(Response_Build, test_Read)
{
    //C1219 table .. TBD
}

TEST_F(Response_Build, test_General)
{
    C1222_Response_General general(0x0);

    uint8_t * d = general.build();

    EXPECT_EQ(0x0, *d);
}


