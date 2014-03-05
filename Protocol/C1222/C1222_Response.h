/* ======================================================================== *
 *                                                                          *
 *                            C1222 Response class                          *  
 *                                                                          *
 * ======================================================================== */

#ifndef __C1222_RESPONSE_H__
#define __C1222_RESPONSE_H__

#include <stdint.h>
#include "C1222.h"

//minimal response code
#define RES_OK      '\x00' //ok 
#define RES_ERR     '\x01' //error 
#define RES_SNS     '\x02' //service not support
#define RES_ONP     '\x04' //operation not possible 
#define RES_BSY     '\x06' //device busy
#define RES_ISSS    '\x0A' //invalid serviece sequence state
#define RES_SME     '\x0B' //security mechanism error 
#define RES_UAT     '\x0C' //unknown application title (aptitle)
#define RES_SGNP    '\x11' //segment not possible


class C1222_Response : public C1222 {
    public:
        C1222_Response();
        ~C1222_Response();

        uint8_t * build(void);
        uint8_t   get_response_num();
        long      get_build_size();

        static void parse(void * data, uint8_t req);
    
    protected:
        uint8_t response_num;
        uint8_t * raw_data; 
        long    buildsize;
};

class C1222_Response_General : C1222_Response {
    public:
        C1222_Response_General(uint8_t res);
};

class C1222_Response_Ident : C1222_Response {
    public:
        C1222_Response_Ident(uint8_t res,  uint8_t std,  
            uint8_t ver, uint8_t rev);

        uint8_t get_std();
        uint8_t get_ver();
        uint8_t get_rev();
    private:
        uint8_t std, ver, rev;
};

class C1222_Response_Read : C1222_Response {
    public:
        C1222_Response_Read(uint8_t res,  unsigned short count,  
            uint8_t * data, const uint8_t chksum); 

        uint8_t * get_data();
        uint8_t   get_chksum();
        unsigned short get_count();

    private:
        uint8_t * data;
        uint8_t chksum;
        unsigned short count;

};

class C1222_Response_Logon : C1222_Response {
    public:
        C1222_Response_Logon(uint8_t res, unsigned short timeout);
    
        unsigned short get_timeout();

    private:
        unsigned short timeout;
};

class C1222_Response_Resolve : C1222_Response {
    public:
        C1222_Response_Resolve(uint8_t res,  uint8_t addr_len, 
                                             uint8_t * addr);

        uint8_t   get_addrlen();
        uint8_t * get_addr();

    private:
        uint8_t   addr_len;
        uint8_t * addr;
};

class C1222_Response_Trace : C1222_Response {
    public:
        C1222_Response_Trace(uint8_t res,  char ** aptitles);

        char ** get_aptitles();

    private:
        char ** aptitles;
};

class C1222_Response_Reg : C1222_Response {
    public:
        C1222_Response_Reg(uint8_t res, char * aptitle, 
           unsigned short delay, long period, uint8_t info);

        char *         get_aptitle();
        long           get_period();
        uint8_t        get_info();
        unsigned short get_delay();

    private:
        char *         ap_title;
        long           period;
        uint8_t        info;
        unsigned short delay;
};



#endif
