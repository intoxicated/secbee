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
        C1222_Response(uint8_t res);
        ~C1222_Response();

        uint8_t * build(void);
        long get_data_len();

        static void parse_response(void * data, uint8_t req);
    
    protected:
        uint8_t * raw_data; 
        long datalen;
};

class C1222_Response_Ident : C1222_Response {
    public:
        C1222_Response_Ident(uint8_t res,  uint8_t std,  
            uint8_t ver, uint8_t rev);
};

class C1222_Response_Read : C1222_Response {
    public:
        C1222_Response_Read(uint8_t res,  uint8_t * count,  
            uint8_t * data, const uint8_t chksum);       
};

class C1222_Response_Logon : C1222_Response {
    public:
        C1222_Response_Logon(uint8_t res, short timeout);
};

class C1222_Response_Resolve : C1222_Response {
    public:
        C1222_Response_Resolve(uint8_t res,  uint8_t addr_len, 
                                             uint8_t * addr);
};

class C1222_Response_Trace : C1222_Response {
    public:
        C1222_Response_Trace(uint8_t res,  char ** aptitles)
};

class C1222_Response_Reg : C1222_Response {
    public:
        C1222_Response_Reg(uint8_t res, char * aptitle, 
                 short delay,  long period, uint8_t info)
};



#endif
