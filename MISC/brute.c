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

#define ID_REQUEST 0x4449

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
        uint16_t temp = (panid & 0xFF00) >> 8;
        uint16_t temp2 = (panid & 0xFF) << 8;
        panid = temp | temp2;    
        printf("id rx: 0x%hx\n", panid);
    }

    else{
        ;
    }
    /*
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

        printf("MY: 0x%hx SH: 0x%x SL: 0x%x\nNI: [%s]\nPARENT: 0x%hxDEVICE: 0x%x STATUS: 0x%x \
                \nPROFILE: 0x%02x  MANU: 0x%02x\n", my, sh, sl,
                ni, parent, device_type, status, profile_id, manu_id);
        didFound = 1;
    }*/
}

int
main (int argc, char ** argv)
{
    struct xbee *xbee;
    struct xbee_con *con;
    xbee_err ret;

    if((ret = xbee_setup(&xbee, "xbee2", "/dev/cu.usbserial-AD025EOA", 9600))
            != XBEE_ENONE)
    {
        printf("set up error\n");
        return ret;
    }

    FILE * log;
    log = fopen("libxbee.log", "w+");
    //xbee_logTargetSet(xbee, log);
    //xbee_logLevelSet(xbee, 100);

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
    
    unsigned int pan_request = 0x4449;
    unsigned short pan_force = 0x0;
    char req_ptr[5];
    unsigned char ret_ptr[1024];
    for(; pan_force < 0xFFFF; pan_force++)
    {
        //init id request
        unsigned short temp = htons(pan_force);
        printf("temp %hx pan %hx \n", temp, pan_force);
        pan_request = ID_REQUEST | (pan_force << 16);

        
        //create char * request with right byte order
        memset(req_ptr, 0x0, 5);
        memcpy(req_ptr, &pan_request, 4);
        req_ptr[4] = '\0';
        int i;
        for (i = 0; i < 4; i++)
            printf(" %02x ", req_ptr[i]);
        puts("");

        //change pan id 
      //  ret = xbee_conTx(con, NULL, "ID1234");
      //  usleep(1000000);
        ret = xbee_conTx(con, NULL, "ID");
        usleep(1000000);
        //neighbor discovery
//        ret = xbee_conTx(con, NULL, "\x49\x44\x11\x11");
//        usleep(4000000);
       ret = xbee_conTx(con, NULL, "AS");
        usleep(4000000);

       ret = xbee_conTx(con, NULL, "ND");
        usleep(4000000);

        if(didFound)
            break;
        break;
    }

    if((ret = xbee_conEnd(con)) != XBEE_ENONE)
    {
        return ret;
    }

    xbee_shutdown(xbee);

    return 0;
}
/*typedef PACKED_STRUCT {
 *  uint8_t as_type;
 * 
 * uint8_t channel;
 * 
 * uint16_t    pan_be;
 * 
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
