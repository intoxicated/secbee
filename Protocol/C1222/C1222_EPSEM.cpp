/* ==================================================================== *
 *                                                                      *
 *                          EPSEM Implementation                        *  
 *                                                                      *
 * ==================================================================== */

#include <string>
#include <stdlib.h>
#include <string.h>

#include "C1222_EPSEM.h"
#include "Utils.hpp"

C1222_EPSEM::C1222_EPSEM()
{

}

C1222_EPSEM::C1222_EPSEM(void * data, uint8_t flag, 
                        int e_class, int datalen)
{
    this->control = flag;
    this->ed_class = e_class;
    this->e_data.data = new uint8_t[datalen];
    memcpy(e_data.data, data, datalen);

    //parse out flags
    this->set_flags(flag);
    this->encoded_size = ber_len_encode(&this->length, datalen, 4);
}

C1222_EPSEM::C1222_EPSEM(const C1222_EPSEM& other)
{
    this->control = other.control;
    this->e_data = other.e_data;

}

C1222_EPSEM::~C1222_EPSEM()
{
    delete this->e_data.data;
}

void
C1222_EPSEM::set_flags(uint8_t flag)
{
    this->reserve = (flag & (1 << 7)) ? true : false;
    this->recovery = (flag & (1 << 6)) ? true : false;
    this->proxy_service = (flag & (1 << 5)) ? true : false;
    this->security_mode = (flag & (3 << 2)) >> 2;
    this->response_mode = (flag & 3);
}

/**
 * Build EPSEM portion of data 
 * Caller responsible to free the memory after used of pointer
 * 
 * @return pointer that contains EPSEM 
 */
uint8_t * 
C1222_EPSEM::build()
{
    //determine total size of epsem 
    int size = 1 + this->encoded_size + this->e_data.size;
    if(this->ed_class != 0x0)
        size += 4;

    uint8_t * builder = new uint8_t[size]; 
    //header
    //encode size
    //class?
    //epsem data

    return NULL;
}

void
C1222_EPSEM::parse(void * data)
{
    uint8_t * ptr = (uint8_t *)data;
    //first byte should be flag 
    uint8_t flag = *ptr;
    int eclass = 0;
    int ber_size, datalen, e_len;

    printf("[!!] EPSEM Parsing start... \n");
    //empty 
    
    if(*ptr == 0x0)
        return;
    
    if((flag >> 4) & 0x1) //if eclass flag is on
    {
        printf("[*] eclass flag copy... \n");
        copy_to(&eclass, ptr+1, 4); 
        printf("[*] get data part... \n");
        e_len = ber_len_decode(ptr+5, &ber_size);
        ptr = ptr + 1 + 4 + ber_size;
    }
    else //if e_class is not present
    {
        printf("[*] get data part... \n");
        e_len = ber_len_decode(ptr+1, &ber_size);
        ptr = ptr + 1 + ber_size;
    }
    printf("[*] data part start as:0x%x and length 0x%x\n", *ptr, e_len);
    
    this->e_data.data = new uint8_t[e_len];
    memcpy(this->e_data.data, ptr, e_len);

    this->e_data.size = e_len;
    this->length = e_len;
    this->ed_class = eclass;
    this->control = flag;
}


    
uint8_t * 
C1222_EPSEM::get_epsem_data()
{
    return this->e_data.data;
}

long
C1222_EPSEM::get_data_len()
{
    return this->e_data.size;
}

void
C1222_EPSEM::set_data(void * data, long len)
{
    if(this->e_data.data != NULL)
        delete this->e_data.data;

    this->e_data.data = new uint8_t[len];
    this->e_data.size = len;

    memcpy(this->e_data.data, (uint8_t*)data, len);
}

long
C1222_EPSEM::get_length()
{
    return this->length;
}

uint8_t
C1222_EPSEM::get_flag(int tag)
{
    if(tag == RESERVE_TAG)
        return this->reserve ? 1 : 0;
    else if(tag == RESERVE_TAG)
        return this->response_mode;
    else if(tag == SECURITY_TAG)
        return this->security_mode;
    else if(tag == PROXY_TAG)
        return this->proxy_service ? 1 : 0;
    else if(tag == RECOVERY_TAG)
        return this->recovery ? 1 : 0;
    
    return -1;
}

