/* ========================================================================= *
 *                                                                           *
 *                                 ACSE class                                *  
 *                                                                           *
 * ========================================================================= */


#ifndef __C1222_ACSE_H__
#define __C1222_ACSE_H__

#include "C1222.h"
/**
 * ACSE format representation class
 */
class C1222_acse : public C1222 {
    public:
        void * build();
        friend C1222_acse& acse_parse(void * data);

    private:
        char * calling_ap_invo;
};

#endif