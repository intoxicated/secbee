/* ========================================================================= *
 *                                                                           *
 *                            ACSE Implementation                            *  
 *                                                                           *
 * ========================================================================= */

#include <string>
#include <iostream>
#include <cstddef>
#include <stdlib.h>

#include "C1222_ACSE.h"
#include "Utils.hpp"

C1222_ACSE::C1222_ACSE()
{

}

C1222_ACSE::C1222_ACSE(void * usrinfo, uint8_t * calling_title, 
        uint8_t * calling_id, uint8_t * called_title, uint8_t * called_id,
        int usrlen)
{
    this->calling_title.data = calling_title;
    this->calling_title.size = 0;

}

C1222_ACSE::C1222_ACSE(void * data)
{

}

C1222_ACSE::~C1222_ACSE()
{

}
void *
C1222_ACSE::build()
{
    return NULL;
}

/* ===================================================================== *
 *                                                                       *
 *                              ACSE Helpers                             *
 *                                                                       *
 * ===================================================================== */

uint8_t *
calling_ap_title_parse(void * data, long * datalen, long * berlen)
{
    return NULL;
}

uint8_t *
calling_id_parse(void * data, long * size)
{
    return NULL;
}

uint8_t *
called_ap_title_parse(void * data, long * datalen, long * berlen)
{
    uint8_t * ptr = (uint8_t *)data;
    int ber_size;

    *datalen = ber_len_decode(ptr, &ber_size);
    *berlen = ber_size;

    return NULL;
    //return ber_uid_decode(ptr, &datalen);
}

uint8_t *
called_id_parse(void * data, long * datalen, long * berlen)
{
    return NULL;
}

uint8_t *
usrinfo_parse(void * data, long * datalen, long * berlen)
{
    uint8_t * ptr = (uint8_t *)data;
    int ber_size;

    *datalen = ber_len_decode(ptr+1, &ber_size);
    ptr = ptr + ber_size; //point user_info_external

    //check header for user-info external
    if(*ptr == 0x28)
    {
        *datalen = ber_len_decode(ptr+1, &ber_size);
        ptr = ptr + ber_size;
    }

    //check header for user info octet string
    if(*ptr == 0x81)
    {
        *datalen = ber_len_decode(ptr+1, &ber_size);
        ptr = ptr + ber_size;
    }

    //now ptr points epsem section
    return ptr;
}


void
C1222_ACSE::parse(void * data)
{

    uint8_t * ptr = (uint8_t *)data;
    uint8_t * calling_at, *calling_id;
    uint8_t * called_at, *called_id;
    uint8_t * usrinfo;
    int datalen, ber_size;

    if(*ptr != 0x60){
        return;
    }
    else {
        datalen = ber_len_decode(ptr + 1, &ber_size);
        ptr = ptr + 1 + ber_size; //now pointing elements
    
        long datalen, berlen;
        
        //parse called
        if(*ptr == 0xA2){
            this->called_title.data = called_ap_title_parse(ptr, &datalen, &berlen);
            this->called_title.size = datalen; 
            ptr = ptr + berlen + datalen;
        }
        
        if(*ptr == 0xA4){
            this->called_id.data = called_id_parse(ptr, &datalen, &berlen);
            this->called_id.size = datalen;
            ptr = ptr + berlen + datalen;
        }
        
        //parse calling
        if(*ptr == 0xA6){
            this->calling_title.data = calling_ap_title_parse(ptr, &datalen, &berlen);
            this->calling_title.size = datalen;
            ptr = ptr + berlen + datalen;
        }
        
        if(*ptr == 0xA8){

        }

        //parse user information
        if(*ptr == 0xBE){
            this->userinfo.data = usrinfo_parse(ptr, &datalen, &berlen);
            ptr = ptr + berlen + datalen;

            this->epsem.parse(this->userinfo.data);
        }
        
        
    }
}

/* getters */

uint8_t * 
C1222_ACSE::get_calling_title()
{
    return NULL;
}

uint8_t * 
C1222_ACSE::get_calling_id()
{
    return NULL;
}
     
uint8_t * 
C1222_ACSE::get_called_title()
{
    return NULL;
}
                
uint8_t * 
C1222_ACSE::get_called_id()
{
    return NULL;
}
 
uint8_t * 
C1222_ACSE::get_usrinfo()
{
    return this->userinfo.data;
}
