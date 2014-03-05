/* ======================================================================== *
 *                                                                          *
 *                            C1222 packet class                            *  
 *                                                                          *
 * ======================================================================== */


#ifndef __C1222_PACKET_H__
#define __C1222-PACKET_H__

#include "C1222.h"
#include "C1222_ACSE.h"
#include "C1222_EPSEM.h"


/**
 * C1222 packet representation 
 */
class C1222_Packet : public C1222 {
    public:
        C1222_packet();

        uint8_t * build();

        C1222_ACSE * get_acse();
        C1222_EPSEM * get_epsem();
        void set_acse(const C1222_ACSE& acseObj);
        void set_epsem(const C1222_EPSEM& epsemObj);

        //wrapper getters/setters
        char * get_calling_title();
        char * get_calling_id();
        char * get_called_title();
        char * get_called_id();

        uint8_t * get_epsem_data();
        void set_data(void * data, long len);
        long get_length();
        long get_data_len();
        uint8_t get_flag(int tag);
        void set_flags(uint8_t flag);

        static C1222_Packet parse(uint8_t * data);

    private:
        void * raw_data;
        int length;
        uint8_t identifier; //require 

        C1222_ACSE * acse;
        C1222_EPSEM * epsem;
};

#endif
