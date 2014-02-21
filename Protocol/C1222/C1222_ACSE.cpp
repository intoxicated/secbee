/* ========================================================================= *
 *                                                                           *
 *                            ACSE Implementation                            *  
 *                                                                           *
 * ========================================================================= */

#include <string>
#include <iostream>

#include "C1222_ACSE.h"
#include "../Utils.h"

C1222_ACSE::C1222_ACSE(void * usrinfo, uint8_t * calling_title, 
        uint8_t * calling_id, uint8_t * called_title, uint8_t * called_id,
        int usrlen)
{
    this->calling_title.data = calling_title;
    this->calling_title.size = 0;

}

C1222_ACSE::~C1222_ACSE()
{
    delete this->calling_id;
    delete this->calling_ap_title;
    delete this->called_id;
    delete this->called_ap_title;
    delete this->userinfo;
}

void *
C1222_ACSE::build()
{

}

/* ===================================================================== *
 *                                                                       *
 *                              ACSE Helpers                             *
 *                                                                       *
 * ===================================================================== */

uint8_t *
calling_ap_title_parse(void * data, long * size)
{
    return NULL;
}

uint8_t *
calling_id_parse(void * data, long * size)
{
    return NULL;
}

uint8_t *
called_ap_title_parse(void * data, long * size)
{
    uint8_t * ptr = (uint8_t *)data + 1;
    int datalen, ber_size;

    datalen = ber_len_decode(ptr, &ber_size);
    ptr = ptr + ber_size;

    return ber_uid_decode(ptr, &datalen);
}

uint8_t *
called_id_parse(void * data, long * size)
{
    return NULL;
}

uint8_t *
usrinfo_parse(void * data, long * size)
{
    return NULL;
}


C1222_ACSE 
acse_parse(void * data)
{
    uint8_t * ptr = (uint8_t *)data;
    uint8_t * calling_at, *calling_id;
    uint8_t * called_at, *called_id;
    uint8_t * usrinfo;
    int datalen, ber_size;

    if(*ptr != 0x60){
        std::cout << "invalid header value ACSE\n";    
        return NULL;
    }
    else {
        datalen = ber_len_decode(ptr + 1, &ber_size);
        ptr = ptr + 1 + ber_size; //now pointing elements
    
        long size;
        if(*ptr == 0xA2){
            calling_at = calling_ap_title_parse(ptr, &size);            
            ptr = ptr + size;
        }

        if(*ptr == 0xA4){
            calling_id = calling_id_parse(ptr, &size);
            ptr = ptr + size;
        }
        
        //parse calling
        if(*ptr == 0xA6){

        }

        if(*ptr == 0xA8){

        }

        if(*ptr == 0xBE){

        }

        C1222_ACSE ret (usrinfo, calling_at, calling_id, called_at, 
                called_id, size);

        return ret;
    }
}
