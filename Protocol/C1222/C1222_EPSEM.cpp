/* ========================================================================= *
 *                                                                           *
 *                            EPSEM Implementation                           *  
 *                                                                           *
 * ========================================================================= */

#include <string>
#include <stdlib.h>
#include <string.h>

#include "C1222_EPSEM.h"
#include "Utils.hpp"

C1222_EPSEM::C1222_EPSEM()
{

}

C1222_EPSEM::C1222_EPSEM(void * data, uint8_t flag, int e_class, int datalen)
{
    this->control = flag;
    this->ed_class = e_class;
    this->e_data_len = datalen;
    this->e_data = new uint8_t[datalen];
    memcpy(e_data, data, datalen);

    //parse out flags
    this->reserve = (flag & (1 << 7)) ? true : false;
    this->recovery = (flag & (1 << 6)) ? true : false;
    this->proxy_service = (flag & (1 << 5)) ? true : false;
    this->security_mode = (flag & (3 << 2)) >> 2;
    this->response_mode = (flag & 3);
}

C1222_EPSEM::C1222_EPSEM(const C1222_EPSEM& other)
{
    this->control = other.control;
    this->e_data = other.e_data;

}

C1222_EPSEM::~C1222_EPSEM()
{
    delete this->e_data;
}

void * 
C1222_EPSEM::build()
{
    return e_data;
}

int
C1222_EPSEM::get_type()
{
    return 0;
}

uint8_t
C1222_EPSEM::get_flag(int flag)
{
    if(flag == RESERVE_TAG)
        return this->reserve ? 1 : 0;
    else if(flag == RESERVE_TAG)
        return this->response_mode;
    else if(flag == SECURITY_TAG)
        return this->security_mode;
    else if(flag == PROXY_TAG)
        return this->proxy_service ? 1 : 0;
    else if(flag == RECOVERY_TAG)
        return this->recovery ? 1 : 0;
    
    return 0;
}

void
C1222_EPSEM::parse(void * data)
{
    uint8_t * ptr = (uint8_t *)data;
    //first byte should be flag 
    uint8_t flag = *ptr;
    int eclass = 0;
    int ber_size, datalen;

    //empty 
    if(*ptr == 0x0)
        ;
    
    if((flag >> 4) & 0x1) //if eclass flag is on
    {
        copy_to(&eclass, ptr+1, 4); 
        datalen = ber_len_decode(ptr+5, &ber_size);
        ptr = ptr + 1 + 4 + ber_size;
    }
    else //if e_class is not present
    {
        ber_len_decode(ptr+1, &ber_size);
        ptr = ptr + 1 + ber_size;
    }

    this->e_data = ptr;
    this->e_data_len = datalen;
    this->ed_class = eclass;
    this->control = flag;
}




uint8_t *
C1222_EPSEM::get_data()
{
    return this->e_data;
}

void
C1222_EPSEM::set_data(void * data, long len)
{
    this->e_data = (uint8_t *)data;
    this->e_data_len = len;
}

long
C1222_EPSEM::get_data_len()
{
    return this->e_data_len;
}
