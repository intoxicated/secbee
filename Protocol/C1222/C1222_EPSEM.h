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
class C1222_epsem : public C1222{
    public:
        C1222_epsem(void * data, int e_class);

        int getType();
        bool getFlags(int flag);
        void * build();

        friend C1222_epsem& epsem_parse(void * data);

    private:
        void * e_data; 
        int header, e_class;
        bool reserved, recovery, proxy, security, response;
};

#endif