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
        C1222_EPSEM(uint8_t * data, uint8_t flag, 
                       int ed_class, int datalen);
        ~C1222_EPSEM();
  
        uint8_t     get_flags();
        long        get_class();
        long        get_service_len();
        
        uint8_t     get_flag(int tag);
        uint8_t *   get_data();
        long        get_build_size();
        long        get_data_len();
        void        set_flags(uint8_t flag);

        uint8_t *            build();
        static C1222_EPSEM * parse(uint8_t * data);
    
    private:    
        element e_data;

        uint8_t * raw_data;
        long  ed_class, build_size, encoded_size, service_len;
        uint8_t control, security_mode, response_mode;
        bool reserve, recovery, proxy_service;
};

#endif
