/* ========================================================================= *
 *                                                                           *
 *                                EPSEM class                                *  
 *                                                                           *
 * ========================================================================= */

#ifndef __C1222_EPSEM_H__
#define __C1222_EPSEM_H__

#include "C1222.h"
/**
 * EPSEM format representation class
 */

#define RESERVE_TAG 0
#define REPONSE_TAG 1
#define SECURITY_TAG 2
#define PROXY_TAG 3
#define RECOVERY_TAG 4

class C1222_EPSEM : public C1222{
    public:
        C1222_EPSEM();
        C1222_EPSEM(void * data, uint8_t flag, int ed_class, int datalen);
        C1222_EPSEM(const C1222_EPSEM& other);
        ~C1222_EPSEM();

        int get_type();
        void set_data(void * data, long len);
        long get_data_len();
        uint8_t * get_data();
        uint8_t get_flag(int flag);

        void * build();
        void parse(void * data);

    private:
        uint8_t * e_data; 
        int e_data_len;

        uint8_t control;
        long  ed_class;
        long  epsem_len; 
        uint8_t security_mode, response_mode;
        bool reserve, recovery, proxy_service;

};

#endif
