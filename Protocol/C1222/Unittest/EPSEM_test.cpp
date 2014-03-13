#include <gtest/gtest.h>
#include <iostream>

#include "../Utils.hpp"
#include "../C1222_Request.h"
#include "../C1222_ACSE.h"
#include "../C1222_EPSEM.h"
#include "../C1222_Response.h"

//TODO

//logon request
uint8_t data[] = "\x80\x0F\x50\x00\x02\
\x55\x53\x45\x52\x20\x4E\x41\x4D\x45\x20\x00\x3C";

//read request
uint8_t data3[] = "\x80\x03\x30\x00\x05";

//logon response
uint8_t data2[] = "\x80\x03\x00\x00\x3c";

class EPSEM_Build : public ::testing::Test {

};

class EPSEM_Parse : public ::testing::Test {

};

TEST_F(EPSEM_Build, test_build)
{
    C1222_Request_Logon logon (2, "USER NAME", 60);
    uint8_t * d = logon.build();

    C1222_EPSEM epsem(d, 0x80, 0, logon.get_build_size());
    uint8_t * ed = epsem.build();

    EXPECT_EQ(0x11, epsem.get_build_size());
    
    uint8_t ctrlflag;
    long serv_len, ber_len = 0;
    long actual_len = 0;
    
    ber_len = ber_len_encode(&serv_len, epsem.get_data_len(), 4);
    
    memcpy(&actual_len, (ed+1), ber_len);
    
    EXPECT_EQ(0x80, *ed);
    EXPECT_EQ(logon.get_build_size(), actual_len);
    
    uint8_t * built_data = ed+1+ber_len;
    uint8_t * actual_data = logon.get_build_data();

    for(int i = 0; i < actual_len; ++i)
        EXPECT_EQ(actual_data[i], built_data[i]);
}

TEST_F(EPSEM_Parse, test_parse)
{
    C1222_EPSEM * epsem = C1222_EPSEM::parse(data);

    EXPECT_EQ(0x0, epsem->get_class());
    EXPECT_EQ(0x80, epsem->get_flags());
    EXPECT_EQ(0x0F, epsem->get_service_len());

    delete epsem;
}

TEST_F(EPSEM_Parse, test_parse2)
{
    C1222_EPSEM * epsem = C1222_EPSEM::parse(data3);

    EXPECT_EQ(0x80, epsem->get_flags());
    EXPECT_EQ(0x03, epsem->get_service_len());
}
