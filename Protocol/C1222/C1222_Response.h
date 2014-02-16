/* ======================================================================== *
 *                                                                          *
 *                            C1222 Response class                          *  
 *                                                                          *
 * ======================================================================== */

#ifndef __C1222_RESPONSE_H__
#define __C1222_RESPONSE_H__

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
        void * build(void);
        void clear(void);
    
        //feature excluded but necessary to add..
        void identify(const byte res, const byte std, const byte ver, 
                const byte rev);  

        void read(const byte res, const byte * count, const byte * data,
                const byte chksum);

        void write(const byte res);

        void logon(const byte res, const byte * timeout);

        void security(const byte res);

        void logoff(const byte res);

        void terminate(const byte res);

        void disconnect(const byte res);

        void wait(const byte res);

        void registration(const byte res, const byte * ap_title, 
                const byte * delay, const byte * period, const byte info);

        void deregistration(const byte res);

        void resolve(const byte res, const byte addr_len, const byte * addr);

        void trace(const byte res, const byte ** aptitles);

    private:
        byte * raw; 
};

#endif
