/* ===================================================================== *
 *                                                                       *
 *                          C1222 Requester proto                        *
 *                                                                       *
 * ===================================================================== */


#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <xbee.h>

#include "../Protocol/C1222/C1222.h"
#include "../Protocol/C1222/C1222_ACSE.h"
#include "../Protocol/C1222/C1222_EPSEM.h"
#include "../Protocol/C1222/C1222_Request.h"
#include "../Protocol/C1222/C1222_Response.h"

#include "Secbee.h"

#define XBEE_TYPE "xbee2"
#define XBEE_BRATE 9600
#define XBEE_DEVICE "/dev/cu.usbserial-AD025EOA"

#define XBEE_ERR(x) xbee_errorToStr(x)

/* Example of logon request
 * after stacking all necessary part it should be looked like this
 *
 * 60 29                                            <acse-pdu>
 *     A2 05                                        <called ap title element>
 *          80 03 7B 75                             <called ap title> = .123.8437
 *     A6 04                                        <calling ap title element>
 *          80 02 7B                                <calling ap title> = .123.4
 *     AB 03                                        <calling ap invo id element> 
 *          02 01 07                                <calling ap invo id> = 7
 *     BE 15                                        <user information element>
 *          28 13                                   <user info external>
 *              81 11                               <user info octet string>
 *                  80                              <epsem control>
 *                  0F                              <service len>
 *                  50                              <logon>
 *                  00 02                           <user id> = 2
 *                  55 53 45 52 20 4E 41 4D 45 20   <user> = "USER NAME"
 *                  00 3C                           <timeout> = 60sec
 */

void
banner()
{

}

void
signal_handler(int signum)
{
    switch(signum)
    {
        case SIGTSTP:
            std::cout << "ctrl-z was pressed" << endl;
            break;
    }
}


void 
myCB(struct xbee * xbee, struct xbee_con * con,
        struct xbee_pkt ** pkt, void ** data)
{
    printf("[**] Receiving phase ... \n");
    if((*pkt)->dataLen == 0)
    {
        printf("[!!] datalen is too short \n");
        return;
    }
    //prepare parsing incoming data
    C1222_ACSE acse;
    C1222_EPSEM * epsem;
    for(int i = 0; i < (*pkt)->dataLen; i++)
    {
        printf("0x%02x ", (*pkt)->data[i]);
    }
    puts("");
    printf("%s\n", (*pkt)->data);
    //parse acse
    acse.parse((*pkt)->data);
    //parse epsem
    if(acse.get_error() != 0x0)
        return;
    epsem = C1222_EPSEM::parse(acse.get_epsem());
    //parse and print response part 
    
    C1222_Response_Logon * lres = C1222_Response_Logon::parse(epsem->get_data());
    printf("[*] Logon Response: %d %d\n", lres->get_response_num(), lres->get_timeout());
    //parse
    if(epsem != NULL)
        delete epsem;
    if(lres != NULL)
        delete lres;
    //delete lres;
}

/**
 * wrapper for setup log
 * 
 * @param log file descripter for log file
 * @param xbee xbee instance that you want to log
 * @return true if successfully set up, otherwise false
 */
bool setupLog(FILE * log, struct xbee * xbee)
{
    //setting up log
    if((log = fopen("libxbee.log", "w+")) == NULL)
    {
        printf("[!] fopen failed\n");
        return false;
    }
    if(xbee_logTargetSet(xbee, log) != XBEE_ENONE)
    {
        printf("[!] xbee_logTargetSet fail\n");
        return false;
    }
    if(xbee_logLevelSet(xbee, 100) != XBEE_ENONE)
    {
        printf("[!] xbee_logLevelSet failed\n");
        return false;
    }
    return true;
}

int main(int argc, char ** argv)
{
    struct xbee * xbee;
    struct xbee_con * con;
    uint8_t txRet;
    xbee_err ret;
    FILE * log;
    struct xbee_conInfo info;

    banner();
    //increase opening files to run libxbee
    system("ulimit -n 1024");

    //set up signal handling
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigaction(SIGTSTP, &sa, NULL);

    printf("[*] Setting up xbee module ... \n");
    if((ret = xbee_setup(&xbee, XBEE_TYPE, XBEE_DEVICE, XBEE_BRATE)) != XBEE_ENONE)
    {
        printf("[!] xbee_setup error(%d): %s\n", ret, XBEE_ERR(ret));
        return ret;
    }
    printf("[*] Succeed to set up xbee instance at %p\n", xbee);
    printf("[*] %-20s: %s\n[*] %-20s: %s\n[*] %-20s: %d\n", "Module type", 
            XBEE_TYPE, "Device", XBEE_DEVICE, "Brates", XBEE_BRATE); 

    if((log = fopen("libxbee.log", "w+")) == NULL)
    {
        printf("[!] fopen failed\n");
        return -1;
    }
    if(xbee_logTargetSet(xbee, log) != XBEE_ENONE)
    {
        printf("[!] xbee_logTargetSet fail\n");
        return -1;
    }
    if(xbee_logLevelSet(xbee, 100) != XBEE_ENONE)
    {
        printf("[!] xbee_logLevelSet failed\n");
        return -1;
    }


    //create connection
    struct xbee_conAddress target_addr;
    memset(&target_addr, 0x0, sizeof(target_addr));
    target_addr.addr64_enabled = 1;
    //memcpy(target_addr.addr64, "\x00\x13\xA2\x00\x40\xA9\x33\x85", 8);
    memcpy(target_addr.addr64, "\x00\x00\x00\x00\x00\x00\xFF\xFF", 8);

    //memcpy(target_addr.addr64, "\x00\x13\xA2\x00\x40\x69\xC4\xA0", 8);
    //setup connection 
    if((ret = xbee_conNew(xbee, &con, "Data", &target_addr)) != XBEE_ENONE){
        printf("[!] xbee_newCon() return :%d(%s)\n", ret, XBEE_ERR(ret));
        xbee_log(xbee, -1, "xbee_newCon() return: %d(%s)", ret, XBEE_ERR(ret));
        return ret;
    }

    if((ret = xbee_conDataSet(con, xbee, NULL)) != XBEE_ENONE){
        printf("[!] xbee_ConDataSet return :%d(%s)\n", ret, XBEE_ERR(ret));
        return ret;
    }

    if((ret = xbee_conCallbackSet(con, myCB, NULL)) != XBEE_ENONE){
        printf("[!] xbee_conCallbackSet return: %d(%s)\n", ret, XBEE_ERR(ret));
        return ret;
    }
    
    struct xbee_conSettings settings;
    xbee_conSettings(con, NULL, &settings);
    settings.disableAck = 1;
    settings.catchAll = 1;
    xbee_conSettings(con, &settings, NULL);

    //construct packet
    C1222_Request_Logon logon(2, "USER NAME", 33);
    uint8_t * d = logon.build(); // build data;

    C1222_EPSEM epsem (d, 0x80, 0, logon.get_build_size());
    d = epsem.build();

    C1222_ACSE acse ( d, "123.4", "7", "123.8437", NULL, epsem.get_build_size());
    d = acse.build();

    //sending phase
    printf("[!] Sending request to Node..\n");
    //ret = xbee_conTx(con, NULL, "HIGH\r\n");
    ret = xbee_connTx(con, NULL, (const unsigned char *)d, acse.get_build_size());
    printf("Ret: %d\n", ret);
    usleep(5000000);

    //ret = xbee_conTx(con, NULL, "LOW\r\n");
    //printf("Ret: %d\n", ret);
    //usleep(3000000);

    //clearing up
    if((ret = xbee_conEnd(con)) != XBEE_ENONE){
        printf("[!] xbee_conEnd return :%d\n", ret);
        return ret;
    }

    xbee_shutdown(xbee);

    return 0;
}
