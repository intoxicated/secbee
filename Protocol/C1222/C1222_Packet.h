/* ======================================================================== *
 *                                                                          *
 *                            C1222 packet class                            *  
 *                                                                          *
 * ======================================================================== */


#ifndef __C1222_PACKET_H__
#define __C1222-PACKET_H__

#include "C1222.h"
/**
 * C1222 packet representation 
 */
class C1222_packet : public C1222 {
    public:
        C1222_packet(char * called_ap, char * called_id, 
            char * calling_ap, char * calling_id, void * data, int len);
        void * build();
        friend C1222_packet& parse(void * data);
    
    private:
        void * data;
        char * called_ap, * called_ap_invo; //destination 
        char * calling_ap, * calling_ap_invo; //source
        int length;
};

#endif