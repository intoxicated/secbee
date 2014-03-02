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
    this->calling_id.size = 0;
    this->calling_title.size = 0;
    this->called_id.size = 0;
    this->called_title.size = 0;
}

C1222_ACSE::C1222_ACSE(void * usrinfo, const char * calling_title, 
        const char * calling_id, const char * called_title, 
        const char * called_id, int usrlen)
{
    this->calling_title.data = (uint8_t *)calling_title;
    this->called_id.data = (uint8_t *)called_id;
    this->calling_id.data = (uint8_t *)calling_id;
    this->called_title.data = (uint8_t *)called_title;
    //this->calling_title.size = 0;
    this->userinfo.data = (uint8_t *)usrinfo;
    this->userinfo.size = usrlen;

}

C1222_ACSE::C1222_ACSE(void * data)
{

}

C1222_ACSE::~C1222_ACSE()
{
    if(this->calling_id.size != 0)
        delete this->calling_id.data;
    if(this->calling_title.size != 0)
        delete this->calling_title.data;
    if(this->called_title.size != 0)
        delete this->called_title.data;
    if(this->called_id.size != 0)
        delete this->called_id.data;
    if(raw_data != NULL)
        delete raw_data;
}

void 
C1222_ACSE::clear()
{

}

/**
 * Build ACSE Packet 
 * ToDo: byte ordering...
 *
 */

uint8_t *
C1222_ACSE::build()
{
    //build userinfo
    printf("User info size : 0x%x\n", userinfo.size);
    long str_len = 0, ext_len = 0, ele_len = 0;
    int str_blen, ublen, eblen;
    str_blen = ber_len_encode(&str_len, userinfo.size, 4);
    ublen = ber_len_encode(&ext_len, userinfo.size + str_blen + 1, 4);
    eblen = ber_len_encode(&ele_len, userinfo.size + str_blen
                        + 1 + ublen + 1, 4);

    printf("strblen : 0x%02x usrblen: 0x%02x external blen : 0x%02x\n", 
        str_blen, ublen, eblen);


    printf("strlen : 0x%02x usrlen: 0x%02x external len : 0x%02x\n", 
        str_len, ext_len, ele_len);

    int usrinfo_len = 1 + ele_len; //total user info length 

    //validate presense of optional 
    if(calling_title.data != NULL)
    {
        printf("CT: %s\n CID: %s\n CllT: %s\n", calling_title.data, 
            calling_id.data, called_title.data);
    }
    
    long cinglen = 0, cedlen = 0, cingidlen = 0;
    int cing_blen, ced_blen, cingid_blen;

    //encode titles/id
    element * cing_id = ber_uid_encode((char *)calling_id.data, 
                                    strlen((char *)calling_id.data), 0x02);

    element * cing_title = ber_uid_encode((char *)calling_title.data,
                                    strlen((char *)calling_title.data), 0x80);

    element * ced_title = ber_uid_encode((char *)called_title.data,
                                    strlen((char *)called_title.data), 0x80);

    //encode length fields
    cing_blen = ber_len_encode(&cinglen, cing_title->size, 4);
    cingid_blen = ber_len_encode(&cingidlen, cing_id->size, 4);
    ced_blen = ber_len_encode(&cedlen, ced_title->size, 4);
    printf("berlen %x %x %x", cing_blen, cingid_blen, ced_blen);
    printf("USER INFO LEN : 0x%x\ncin %x cid %x ced %x\n", usrinfo_len
        ,cinglen, cingidlen, cedlen);
    int pdu_size = usrinfo_len + cinglen + cingidlen + cedlen + 3 +
        cing_blen + cingid_blen + ced_blen;

    printf("PDU SIZE 0x%x\n", pdu_size);

    int pdulen = 0; long pdublen = 0;
    pdublen = ber_len_encode(&pdulen, pdu_size, 4);

    int totalSize = 1 + pdulen + pdublen;

    printf("Totalsize is 0x%x\n", totalSize);

    //now copy into buffer
    raw_data = new uint8_t[totalSize];

    //first header
    raw_data[0] = 0x60;
    //encoded len copy
    memcpy(raw_data + 1, &pdulen, pdublen);

    //ap called title copy
    int sum = 1 + pdublen;

    raw_data[sum] = 0xA2;
    memcpy(raw_data + sum + 1, &cedlen, ced_blen);
    memcpy(raw_data + sum + 1 + ced_blen, ced_title->data, ced_title->size);

    raw_data[sum] = 0xA6;
    //copy calling title
    raw_data[sum] = 0xA8;
    //copy calling id
    raw_data[sum] = 0xBE;
    //copy user info len
    raw_data[sum] = 0x28;
    //copy external len
    raw_data[sum] = 0x81;
    //copy octet str len

    //copy userinfo data
    memcpy(raw_data + sum, userinfo.data, userinfo.size);
    //free up memory....... better manangement needs
    //TO do

    return raw_data;
}

/* ===================================================================== *
 *                                                                       *
 *                              ACSE Helpers                             *
 *                                                                       *
 * ===================================================================== */

uint8_t *
ap_title_parse(void * data, long * datalen, long * berlen)
{
    uint8_t * ptr = (uint8_t *)data;
    int ber_size;

    *datalen = ber_len_decode(ptr + 1, &ber_size);
    *berlen = ber_size;
    ptr = ptr + 1 + *berlen; 
   
    return (*ptr == 0x80 || *ptr == 0x02) ?
        (uint8_t *)ber_uid_decode(ptr+1) : NULL;
    
}

uint8_t *
usrinfo_parse(void * data, long * datalen, long * berlen)
{
    uint8_t * ptr = (uint8_t *)data;
    int ber_size;

    *datalen = ber_len_decode(ptr + 1, &ber_size);
    ptr = ptr + 1 + ber_size; //point user_info_external
    printf(" [--->] userinfo element size : 0x%lx\n", *datalen);
    //check header for user-info external
    if(*ptr == 0x28)
    {
        printf("[--->] check user info external \n");
        *datalen = ber_len_decode(ptr+1, &ber_size);
        ptr = ptr + 1 + ber_size;
        printf(" [--->] external size : 0x%lx\n", *datalen);
    }

    //check header for user info octet string
    if(*ptr == 0x81)
    {
        printf("[--->] check user info octet string ..\n");
        *datalen = ber_len_decode(ptr+1, &ber_size);
        ptr = ptr + 1 + ber_size;
        printf(" [--->] octet size : 0x%lx\n", *datalen);
    }

    //now ptr points epsem section
    return ptr;
}

void
usrinfo_build(void * data, long size)
{
    long str_len, usrinfo_len, external_len;
    int berlen;
    berlen = ber_len_encode(&str_len, size, 4);



}

void
C1222_ACSE::parse(void * data)
{

    uint8_t * ptr = (uint8_t *)data;
    uint8_t * calling_at, *calling_id;
    uint8_t * called_at, *called_id;
    uint8_t * usrinfo;
    int datalen, ber_size;

    printf("ACSE parsing start..\n");
    if(*ptr != 0x60){
        return;
    }
    else {
        printf("Decoding length of ACSE..\n");
        datalen = ber_len_decode(ptr + 1, &ber_size);
        ptr = ptr + 1 + ber_size; //now pointing elements
        printf("data length is : %x \n", datalen);
        long datalen, berlen;
        
        //parse called
        if(*ptr == 0xA2){
            printf("[*] Parsing called title... \n");
            this->called_title.data = ap_title_parse(ptr, &datalen, &berlen);
            this->called_title.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
        }

        printf("[---->] Called ap title is : %s\n", this->called_title.data);
   
        //check first
        if(*ptr == 0xA4){
            this->called_id.data = ap_title_parse(ptr, &datalen, &berlen);
            this->called_id.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
        }
        
        //parse calling
        if(*ptr == 0xA6){
            printf("[*] Parsing calling title... \n");
            this->calling_title.data = ap_title_parse(ptr, &datalen, &berlen);
            this->calling_title.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
        }
        
        printf("[---->] Calling ap title is : %s\n", this->calling_title.data);
         
        if(*ptr == 0xA8){
            printf("[*] Parsing calling invo id... \n");
            this->calling_id.data = ap_title_parse(ptr, &datalen, &berlen);
            this->calling_id.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
        }
        
        printf("[---->] Calling invo id is : %s\n", this->calling_id.data);
         
        //parse user information
        if(*ptr == 0xBE){
            printf("[*] Parsing user info ... \n");
            //this->userinfo.data = ptr;
            this->userinfo.data = usrinfo_parse(ptr, &datalen, &berlen);
            this->userinfo.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
            printf("[!] userinfo data start with : 0x%x and length 0x%lx\n", *(this->userinfo.data)
                    , datalen);
        }
        
        
    }
    printf("[!!] ACSE Parsing done!\n");
}

/* getters */

char * 
C1222_ACSE::get_calling_title()
{
    return NULL;
}

char * 
C1222_ACSE::get_calling_id()
{
    return NULL;
}
     
char * 
C1222_ACSE::get_called_title()
{
    return NULL;
}
                
char * 
C1222_ACSE::get_called_id()
{
    return NULL;
}
 
uint8_t * 
C1222_ACSE::get_epsem()
{
    return this->userinfo.data;
}

long
C1222_ACSE::get_data_len()
{
    return this->acse_len;
}

void
C1222_ACSE::set_epsem(void * data, long size)
{
    usrinfo_build(data, size);
}


