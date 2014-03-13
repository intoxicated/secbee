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
    build_size = 0;
    e_data.size = 0;
    raw_data = NULL;
}

C1222_EPSEM::C1222_EPSEM(uint8_t * data, uint8_t flag, 
                        int e_class, int datalen)
{
    control = flag;
    ed_class = e_class;
    e_data.size = datalen;
    e_data.data = new uint8_t[datalen];
    raw_data = NULL;
    service_len = datalen;

    memcpy(e_data.data, data, datalen);

    //parse out flags
    this->set_flags(flag);
}

C1222_EPSEM::C1222_EPSEM(const C1222_EPSEM& other)
{
    this->control = other.control;
    this->e_data = other.e_data;

}

C1222_EPSEM::~C1222_EPSEM()
{
    if(e_data.size != 0)
        delete this->e_data.data;
    if(raw_data != NULL)
        delete raw_data;
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
    printf( "[*] Building EPSEM data...\n");
    
    if(raw_data != NULL)
        delete raw_data;

    //encode service length
    encoded_size = ber_len_encode(&service_len, e_data.size, 4);

    //calculate buffer size
    if(ed_class != 0x0)
        build_size = encoded_size + 1 + 4 + e_data.size;
    else
        build_size = encoded_size + 1 + e_data.size;

    int offset = 1;
    raw_data = new uint8_t[build_size]; 
    raw_data[0] = control;

    //add class if present
    if(ed_class != 0x0){
        memcpy(raw_data+1, &ed_class, 4);
        offset += 4;
    }
    else{
        printf("    [!] eclass is not present\n");
    }
        
    //encode size
    memcpy(raw_data + offset, &service_len, encoded_size);
    offset += encoded_size;
    //epsem data
    memcpy(raw_data + offset, e_data.data, e_data.size);
    
    return raw_data;
}

C1222_EPSEM *
C1222_EPSEM::parse(uint8_t * data)
{
    uint8_t * ptr = (uint8_t *)data;
    //first byte should be flag 
    uint8_t flag = *ptr;
    int eclass = 0;
    int ber_size = 0, serv_len = 0;

    printf("[!!] EPSEM Parsing start... \n");
    //empty 
    
    if(*ptr == 0x0)
        return NULL;
    
    if((flag >> 4) & 0x1) //if eclass flag is on
    {
        printf("[*] eclass flag copy... \n");
        copy_to(&eclass, ptr+1, 4); 
        printf("[*] get data part... \n");
        serv_len = ber_len_decode(ptr+5, &ber_size);
        ptr = ptr + 1 + 4 + ber_size;
    }
    else //if e_class is not present
    {
        printf("[*] get data part... \n");
        serv_len = ber_len_decode(ptr+1, &ber_size);
        ptr = ptr + 1 + ber_size;
    }
    printf("[*] data part start as:0x%x and \
            length 0x%x\n", *ptr, serv_len);

    return new C1222_EPSEM(ptr, *data, eclass, serv_len);
}


long
C1222_EPSEM::get_service_len()
{
    return service_len;
}

uint8_t 
C1222_EPSEM::get_flags()
{
    return control;
}

long
C1222_EPSEM::get_class()
{
    return ed_class;
}

uint8_t * 
C1222_EPSEM::get_data()
{
    return e_data.data;
}

long
C1222_EPSEM::get_data_len()
{
    return e_data.size;
}

long
C1222_EPSEM::get_build_size()
{
    return build_size;
}

uint8_t
C1222_EPSEM::get_flag(int tag)
{
    if(tag == RESERVE_TAG)
        return reserve ? 1 : 0;
    else if(tag == RESERVE_TAG)
        return response_mode;
    else if(tag == SECURITY_TAG)
        return security_mode;
    else if(tag == PROXY_TAG)
        return proxy_service ? 1 : 0;
    else if(tag == RECOVERY_TAG)
        return recovery ? 1 : 0;
    
    return -1;
}

