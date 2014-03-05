/* ==================================================================== *
 *                                                                      *
 *                           EPSEM class                                *  
 *                                                                      *
 * ==================================================================== */

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

        uint8_t * build();
        void parse(void * data);
        
        uint8_t * get_data();
        void set_data(void * data, long len);
        long get_length();
        long get_data_len();
        uint8_t get_flag(int tag);
        void set_flags(uint8_t flag);
    
    private:    
        element e_data;

        uint8_t * raw_data;
        long  ed_class, length, encoded_size, service_len;
        uint8_t control, security_mode, response_mode;
        bool reserve, recovery, proxy_service;
};

#endif
