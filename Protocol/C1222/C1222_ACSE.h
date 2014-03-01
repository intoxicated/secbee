/* ========================================================================= *
 *                                                                           *
 *                                 ACSE class                                *  
 *                                                                           *
 * ========================================================================= */


#ifndef __C1222_ACSE_H__
#define __C1222_ACSE_H__

#include <stdint.h>

#include "C1222.h"
#include "C1222_EPSEM.h"

/**
 * ACSE format representation class
 */
class C1222_ACSE : public C1222 {
    public:
        C1222_ACSE();
        C1222_ACSE(const C1222_ACSE& other);
        C1222_ACSE(void * data);
        C1222_ACSE(void * usrinfo, uint8_t * calling_title, uint8_t * calling_id,
                uint8_t * called_title, uint8_t * called_id, int usrlen);
        ~C1222_ACSE();
                
        void * build();
        void clear();

        void parse(void * data);

        char * get_calling_title();
        char * get_calling_id();
        char * get_called_title();
        char * get_called_id();
        uint8_t * get_epsem();
        long get_data_len();
        
        void set_epsem(void * data, long size);
    private:
        element calling_title, calling_id;
        element called_title, called_id;
        element userinfo;

        long acse_len;
};

#endif
