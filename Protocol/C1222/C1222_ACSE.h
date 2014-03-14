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
 * minimal packet format overview
 *
 * ACSE PDU
 * ---------------------------
 * | 0x60 | length | element |
 * ---------------------------
 *
 * element                       if segment
 * -----------------------------  ---------------
 * |   ap element  | user-info |  seg user info |
 * -----------------------------  ---------------
 *
 * ap element 
 * ----------------------------------------------------------------------
 * |  tag | int length (bytes) | 0x02 | id length (bytes) | id (bytes)  | 
 * ----------------------------------------------------------------------
 *
 * user-info
 * --------------------------------------------------------
 * | 0xBE | ext len (4bytes) | 0x28 | info len (4bytes)   |
 * --------------------------------------------------------
 * | 0x81 | oct str len (4bytes) |         epsem          |
 * --------------------------------------------------------
 *
 * epsem
 * -------------------------------------------------------
 * | epsem ctrl (8bits)| serv len (bytes)   | req / res  | 
 * -------------------------------------------------------
 *
 */

class C1222_ACSE : public C1222 {
    public:
        C1222_ACSE();
        C1222_ACSE(void * usrinfo, const char * calling_title, 
            const char * calling_id, const char * called_title, 
            const char * called_id, int usrlen);
        ~C1222_ACSE();
                
        uint8_t * build();

        void parse(void * data);

        char * get_calling_title();
        char * get_calling_id();
        char * get_called_title();
        char * get_called_id();

        uint8_t * get_epsem();
        long get_build_size();
        
        void set_epsem(void * data, long size);

    private:
        element calling_title, calling_id;
        element called_title, called_id;
        element userinfo;

        uint8_t * raw_data;
        long acse_len;
};

#endif
