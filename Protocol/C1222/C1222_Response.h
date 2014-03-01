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
        void * build(void);
        void clear(void);
        long get_data_len();

        //feature excluded but necessary to add..
        void identify(const uint8_t res, const uint8_t std, const uint8_t ver, 
                const uint8_t rev);  

        void read(const uint8_t res, const uint8_t * count, const uint8_t * data,
                const uint8_t chksum);

        void write(const uint8_t res);

        void logon(const uint8_t res, const uint8_t * timeout);

        void security(const uint8_t res);

        void logoff(const uint8_t res);

        void terminate(const uint8_t res);

        void disconnect(const uint8_t res);

        void wait(const uint8_t res);

        void registration(const uint8_t res, const uint8_t * ap_title, 
                const uint8_t * delay, const uint8_t * period, const uint8_t info);

        void deregistration(const uint8_t res);

        void resolve(const uint8_t res, const uint8_t addr_len, const uint8_t * addr);

        void trace(const uint8_t res, const uint8_t ** aptitles);

        static void parse_response(void * data);
    private:
        uint8_t * raw; 
        long datalen;
};

#endif
