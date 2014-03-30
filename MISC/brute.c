/**
 * Bruteforce pan ID to identify neighbor devices
 *
 * Wooyoung Chung
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>

#include <xbee.h>

#define ID_REQUEST 0x49440000

int didFound = 0;    

void
callback(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt,
        void ** data)
{
    if((*pkt)->dataLen == 0)
    {
        return;
    }
    
    if((*pkt)->dataLen == 8){
        uint16_t panid;
        memcpy(&panid, (*pkt)->data + 6, 2);
        panid = ntohs(panid);    
        printf("0x%04x\n", panid);
    }
    else if((*pkt)->dataLen == 16)
    {
        int i;
        printf("Active Scan result:\n");
        for (i =0 ; i < 15; i++)
            printf(" 0x%02x ", (*pkt)->data[i]);
        puts("");
    }
    //actual ATND result parsing
    else if((*pkt)->dataLen >= 20){
        unsigned short my;
        uint32_t sl,sh;
        char ni[30];
        unsigned short parent, profile_id, manu_id;
        uint8_t device_type, status;
        int ni_len;

        memcpy(&my, (*pkt)->data, 2);
        memcpy(&sh, (*pkt)->data+2, 4);
        memcpy(&sl, (*pkt)->data+6, 4);
        strcpy(ni, (char *)(*pkt)->data + 10);

        ni_len = strlen(ni) + 1;
         
        memcpy(&parent, (*pkt)->data + 10 + ni_len, 2);
        memcpy(&device_type, (*pkt)->data + 12 + ni_len, 1);
        memcpy(&status, (*pkt)->data + 13 + ni_len, 1);

        memcpy(&profile_id, (*pkt)->data + 14 + ni_len, 2);
        memcpy(&manu_id, (*pkt)->data + 16 + ni_len, 2);
    
        //byte order swap
        my = my >> 8 | my << 8;
        sh = (sh & 0xFF000000) >> 24 | (sh & 0xFF0000) >> 8 
            | (sh & 0xFF00) << 8 | (sh & 0xFF) << 24;
        sl = (sl & 0xFF000000) >> 24 | (sl & 0xFF0000) >> 8 
            | (sl & 0xFF00) << 8 | (sl & 0xFF) << 24;
        parent = parent >> 8 | parent << 8;
        profile_id = profile_id >> 8 | profile_id << 8;
        manu_id = manu_id >> 8 | manu_id << 8;
        
        printf("Neighbor Discovery result:\n");
        printf("MY: 0x%hx SH: 0x%x SL: 0x%x\nNI: [%s]\nPARENT: 0x%hxDEVICE: 0x%x STATUS: 0x%x \
                \nPROFILE: 0x%02x  MANU: 0x%02x\n", my, sh, sl,
                ni, parent, device_type, status, profile_id, manu_id);
        didFound = 1;
    }
}

int
main (int argc, char ** argv)
{
    struct xbee *xbee;
    struct xbee_con *con;
    xbee_err ret;

    FILE * fd;
    unsigned int pan_request = 0x49440000;
    unsigned short pan_force = 0x0;
    unsigned char req_ptr[5];
    unsigned char ret_ptr[1024];
    FILE * log;

restart:
    fd = fopen("scan.log", "w+");
    if((ret = xbee_setup(&xbee, "xbee2", "/dev/cu.usbserial-AD025EOA", 9600))
            != XBEE_ENONE)
    {
        printf("set up error\n");
        return ret;
    }

    log = fopen("libxbee.log", "w+");
    xbee_logTargetSet(xbee, log);
    xbee_logLevelSet(xbee, 100);
    
    if((ret = xbee_conNew(xbee, &con, "Local AT", NULL)) != XBEE_ENONE)
    {
        printf("conNew error\n");
        return ret;
    }

    if((ret = xbee_conCallbackSet(con, callback, NULL)) != XBEE_ENONE)
    {
        printf("xbee_conCallbackSet error\n");
        return ret;
    }
    
    struct xbee_conSettings settings;
    xbee_conSettings(con, NULL, &settings);  
    settings.extendTimeout = 1;
    xbee_conSettings(con, &settings, NULL);
    
    printf("Check current PAN ID ..\n");
    xbee_conTx(con, NULL, "ID");
    usleep(1000000);
    
    printf("Change pan id... to 0x%04x\n", argv[1]);
    xbee_conTx(con, NULL, argv[1]);
    usleep(1000000);

    usleep(12000000);

    printf("Recheck\n");
    xbee_conTx(con, NULL, "ND");
    usleep(5000000);
    exit(1); 
    while(1)
    {
        //init id request
        fprintf(fd, "[*] Testing pan id 0x%04x :", pan_force); 
        printf("\rAttempting 0x%04x", pan_force);
        fflush(stdout);
        pan_request = ID_REQUEST | (pan_force);
        pan_request = htonl(pan_request);
        //create char * request with right byte order
        memset(req_ptr, 0x0, 5);
        memcpy(req_ptr, &pan_request, 4);
        req_ptr[4] = '\0';

        //change pan id 
        ret = xbee_connTx(con, NULL, (const unsigned char *)req_ptr, 4);
        usleep(1000000);
        
        if(ret != 0x0){
            fprintf(fd, "[!!] out of frame id.. reset\n");
            break;
        }

        ret = xbee_conTx(con, NULL, "ND");
        usleep(5000000);
        
        //neighbor discovery
        ret = xbee_conTx(con, ret_ptr, "ND");
        usleep(5000000);
        if(didFound)
            break;
        else {
            fprintf(fd, " Did not find a neighbor..\n");
            fflush(fd);
        }
        //increment pan id 
        pan_force++;
    }
exit:
    fclose(fd);

    if((ret = xbee_conEnd(con)) != XBEE_ENONE)
    {
        return ret;
    }

    xbee_shutdown(xbee);

    usleep(2000000);

    //alternative solution for frame ID incrementation
    //if(pan_force < 0xFFFF)
    //    goto restart;
    
    return 0;
}

//active scan result
/*typedef PACKED_STRUCT {
 *  uint8_t as_type;
 * 
 * uint8_t channel;
 * 
 * uint16_t    pan_be;
 * :
 * addr64  extended_pan_be;
 * 
 * uint8_t allow_join;
 * 
 * uint8_t stack_profile;
 * 
 * uint8_t lqi;
 * 
 * int8_t  rssi;
 * 
 * } xbee_atas_response_t;
 */
