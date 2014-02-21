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
        friend C1222_ACSE acse_parse(void * data);


    private:
        uint8_t * calling_ap_title, * calling_id;
        uint8_t * called_ap_title, * called_id;
        uint8_t * userinfo;

        container calling_title;

        C1222_EPSEM epsem;
};

#endif
