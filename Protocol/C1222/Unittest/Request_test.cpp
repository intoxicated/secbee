#include <gtest/gtest.h>
#include <iostream>
#include <stdlib.h>

#include "../C1222_Request.h"
#include "../C1222_EPSEM.h"
#include "../C1222_ACSE.h"

uint8_t data[] = "\x60\x29\xA2\x05\x80\x03\x7B\xC1\x75\
\xA6\x04\x80\x02\x7B\x04\xA8\x03\x02\x01\x07\
\xBE\x15\x28\x13\x81\x11\x80\x0F\x50\x00\x02\
\x55\x53\x45\x52\x20\x4E\x41\x4D\x45\x20\x00\x3C";

uint8_t data2[] = "\x60\x22\xa2\x04\x80\x02\x7b\x04\
\xa4\x03\xa6\x05\x80\x03\x7b\xc1\x75\xa8\x03\x02\x01\x07\
\xbe\x09\x28\x07\x81\x05\x80\x03\x00\x00\x3c";

class Request_Build : public ::testing::Test {

};

class Request_Parse : public ::testing::Test {

};

TEST_F(Request_Build, Test_Read)
{
    C1222_Request_Read read(0x30, 0x1, NULL, 0, 0);
    
    //check built data
    uint8_t * d = read.build();

    short tableid;
    memcpy(&tableid, d+1, 2);
    tableid = ntohs(tableid);

    EXPECT_EQ(0x30, *d);
    EXPECT_EQ(0x1, tableid);
}

TEST_F(Request_Build, Test_Write)
{
   //TBD 
}

TEST_F(Request_Build, Test_Logon)
{
    C1222_Request_Logon logon (2, "USER NAME", 60);

    uint8_t * d = logon.build(); 
    
    uint8_t reqn;
    short timeout, id;
    char uname[11];

    memcpy(&reqn, d, 1);
    memcpy(&id, d+ 1, 2);
    memcpy(&timeout, d + 13, 2);
    memcpy(uname, d + 3, 10);
    uname[10] = '\0';

    timeout = ntohs(timeout);
    id = ntohs(id);

    EXPECT_EQ(0x50, reqn);
    EXPECT_EQ(2, id);
    EXPECT_EQ(60, timeout);
    EXPECT_STREQ("USER NAME\x20", uname); 
    //check built data
}

TEST_F(Request_Build, Test_LogOff)
{
    C1222_Request_Logoff logoff;
    uint8_t * d = logoff.build();

    EXPECT_EQ(0x52, *d);
}

TEST_F(Request_Build, Test_Id)
{
    C1222_Request_Ident ident;
    uint8_t * d = ident.build();

    EXPECT_EQ(0x20, *d);
}

TEST_F(Request_Build, Test_Wait)
{
    C1222_Request_Wait wait(0x2);
    uint8_t * d = wait.build();

    EXPECT_EQ(0x70, *d);
    EXPECT_EQ(0x2, *(d+1));
}

TEST_F(Request_Build, Test_Resolve)
{
    C1222_Request_Resolve resolve("123.4");
    uint8_t * d = resolve.build();
    char encoded[5];

    memcpy(encoded, d+1, 4);

    encoded[4] = '\0';

    EXPECT_EQ(0x25, *d);
    EXPECT_STREQ("\x80\x02\x7B\x04", encoded); 
}

TEST_F(Request_Build, Test_Trace)
{
    C1222_Request_Trace trace("123.8437");
    uint8_t * d = trace.build();
    char encoded[6];

    memcpy(encoded, d+1, 5);

    encoded[5] = '\0';

    EXPECT_EQ(0x26, *d);
    EXPECT_STREQ("\x80\x03\x7B\xC1\x75", encoded);
}

TEST_F(Request_Build, Test_Security)
{
    C1222_Request_Security security("PASSWORD..", 0x3);
    uint8_t * d = security.build();

    char pw[21];
    short uid;
    memcpy(pw, d+1, 20);
    memcpy(&uid, d+21,2);

    uid = ntohs(uid);
    pw[10] = '\0';

    EXPECT_EQ(0x51, *d);
    EXPECT_STREQ("PASSWORD..", pw);
    EXPECT_EQ(0x3, uid);
}

/** PARSE **/
TEST_F(Request_Parse, Test_Read)
{
    uint8_t d[] = "\x30\x01";

    C1222_Request_Read * read =
        (C1222_Request_Read *)C1222_Request_Parser::parse(d);

    EXPECT_EQ(0x1, read->get_tableid());

    delete read;
}

TEST_F(Request_Parse, Test_Write)
{
    //TODO
}

TEST_F(Request_Parse, Test_Logon)
{
    uint8_t d[] = "\x50\x00\x02\x55\x53\x45\x52\x20\x4E\
\x41\x4D\x45\x20\x00\x3C";
    C1222_Request_Logon * logon =
        (C1222_Request_Logon *)C1222_Request_Parser::parse(d);

    EXPECT_EQ(0x2, logon->get_user_id());
    EXPECT_EQ(0x3C, logon->get_timeout());
    EXPECT_STREQ("USER NAME\x20", logon->get_username());
    delete logon;
}

TEST_F(Request_Parse, Test_Trace)
{
    uint8_t d[] = "\x26\x80\x02\x7B\x04";

    C1222_Request_Resolve * trace = 
        (C1222_Request_Resolve *)C1222_Request_Parser::parse(d);

    EXPECT_STREQ("123.4", trace->get_ap_title());
    delete trace;
}

TEST_F(Request_Parse, Test_Resolve)
{
    uint8_t d[] = "\x25\x80\x02\x7B\x04";

    C1222_Request_Resolve * resolve = 
        (C1222_Request_Resolve *)C1222_Request_Parser::parse(d);

    EXPECT_STREQ("123.4", resolve->get_ap_title());
    delete resolve;
}

TEST_F(Request_Parse, Test_Wait)
{
    uint8_t d[] = "\x70\x50";

    C1222_Request_Wait * wait = 
        (C1222_Request_Wait *)C1222_Request_Parser::parse(d);

    EXPECT_EQ(0x50, wait->get_interval());
    delete wait;
}


