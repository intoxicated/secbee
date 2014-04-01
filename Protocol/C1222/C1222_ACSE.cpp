/* ==================================================================== *
 *                                                                      *
 *                          ACSE Implementation                         *  
 *                                                                      *
 * ==================================================================== */

#include <string>
#include <iostream>
#include <cstddef>
#include <stdlib.h>

#include "C1222_ACSE.h"
#include "Utils.hpp"

/**
 * Default constructor
 */
C1222_ACSE::C1222_ACSE()
{
    this->calling_id.size = 0;
    this->calling_title.size = 0;
    this->called_id.size = 0;
    this->called_title.size = 0;
    this->raw_data = NULL;
}

/**
 * Parameter constructor
 * 
 * @param usrinfo (require) pointer to user information section
 * @param calling_title (optional) string of calling title (decoded) 
 * @param calling_id (require) string of calling id (decoded)
 * @aaram called_title (optional) string of called title (decoded)
 * @param called_id (optional) string of called title (decoded)
 * @param usrlen (require) length of user information section 
 *
 */
C1222_ACSE::C1222_ACSE(void * usrinfo, const char * calling_title, 
        const char * calling_id, const char * called_title, 
        const char * called_id, int usrlen)
{

    if(calling_title != NULL){
        this->calling_title.size = strlen(calling_title);
        this->calling_title.data = 
                new uint8_t[this->calling_title.size+1];
        this->calling_title.data[this->calling_title.size] = '\0';

        memcpy(this->calling_title.data, calling_title, 
            this->calling_title.size);

    } else {
        this->calling_title.data = NULL;
        this->calling_title.size = 0;
    }

    if(called_title != NULL){
        this->called_title.size = strlen(called_title);
        this->called_title.data = 
                new uint8_t[this->called_title.size+1];
        this->called_title.data[this->called_title.size] = '\0';

        memcpy(this->called_title.data, called_title, 
            this->called_title.size);

    } else {
        this->called_title.data = NULL;
        this->called_title.size = 0;
    }

    if(called_id != NULL){
        this->called_id.size = strlen(called_id);
        this->called_id.data = new uint8_t[strlen(called_id)+1];
        this->called_id.data[this->called_id.size] = '\0';

        memcpy(this->called_id.data, called_id, this->called_id.size);

    } else {
        this->called_id.data = NULL;
        this->called_id.size = 0;
    }

    this->calling_id.size = strlen(calling_id);
    this->calling_id.data = new uint8_t[this->calling_id.size+1];
    this->calling_id.data[this->calling_id.size] = '\0';

    memcpy(this->calling_id.data, calling_id, this->calling_id.size);
    
    this->userinfo.data = (uint8_t *)usrinfo;
    this->userinfo.size = usrlen;
}

/**
 * Destructor
 */
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


/* ================================================================ *
 *                                                                  *
 *                         Build Helpers                            *
 *                                                                  *
 * ================================================================ */

/**
 * build helper
 * clean up ap element
 *
 * @param ptr ap element
 * @param name name of ap element (debug)
 */
void
cleanup(element * ptr, const char * name)
{

#ifdef DEBUG
    printf("    [!] Cleaning %s\n", name);
#endif

    if(ptr != NULL)
    {
        delete ptr->data;
        delete ptr;
    }
}

/**
 * build helper 
 * add ap element to the buffer
 *
 * @param buf rawdata buffer
 * @param header header of ap element
 * @param e_len ap element's encoded length
 * @param e_blem size of ap element's encoded length 
 * @param element ap element 
 * @param offset offset to be used for buffer
 */
void
addAPtoRawData(uint8_t * buf, uint8_t header, 
    long e_len, int e_blen, element * value,  int& offset)
{
    if(value != NULL){
        buf[offset++] = header;
        memcpy(buf + offset, &e_len, e_blen);

        offset += e_blen;
        memcpy(buf + offset, value->data, value->size);
        offset += value->size;
    }    
}

/**
 * build helper
 * add user information element to the buffer
 *
 */
void
addUserInfoToRawData(void * data, long size)
{
    
}

/* ================================================================ *
 *                                                                  *
 *                         Build Helpers End                        *
 *                                                                  *
 * ================================================================ */


/**
 * Build ACSE frame
 *
 * @return raw_data buffer that contains ACSE data
 */
uint8_t *
C1222_ACSE::build()
{
    ///////////////////////////////////////////////////////////////
    //                                                           //
    //              calculate all sizes and offsets              //
    //               and encoded datas (bottom up)               //
    //                                                           //
    ///////////////////////////////////////////////////////////////

    //calculate user info section 
#ifdef DEBUG
    printf("[*] Building ACSE data ...\n");
    printf("    [*] User info size : 0x%x\n", userinfo.size);
#endif
    long str_len = 0, ext_len = 0, ele_len = 0;
    int str_blen, ublen, eblen;

    str_blen = ber_len_encode(&str_len, userinfo.size, 4);
    ublen = ber_len_encode(&ext_len, userinfo.size + str_blen + 1, 4);
    eblen = ber_len_encode(&ele_len, userinfo.size + str_blen
                        + 1 + ublen + 1, 4);
#ifdef DEBUG
    printf("    [!] calculated userinfo fields\n");
#endif
    int usrinfo_len = 1 + ele_len; //total user info length 

    //encode uid variables
    element * e_calling_id = NULL, * e_calling_title = NULL;
    element * e_called_id = NULL, * e_called_title = NULL;

    //variables for encoded length 
    long calling_t_len = 0, calling_id_len = 0;
    long  called_t_len = 0, called_id_len = 0;

    //variables for size of encoded length 
    int calling_t_blen = 0, calling_id_blen = 0;
    int called_t_blen = 0, called_id_blen = 0;
#ifdef DEBUG
    printf("    [!] calculating id/title fields\n");
#endif
    //TODO: refactor this part 
    //0x02 is tag for id 0x80 is tag for title 
    e_calling_id = ber_uid_encode((char *)calling_id.data, 
                                    calling_id.size, 0x02);

    //ber_len_encode return size of encoded length 
    //first argument will have encoded length 
    //last argument is maximum byte (4 bytes for now )
    calling_id_blen = ber_len_encode(&calling_id_len, 
                                    e_calling_id->size, 4);
     //cumulative pdu size so far 
    //usrinfo len + header of calling_id which is required.
    int pdusize = usrinfo_len + 1 + calling_id_blen;

    if(calling_title.size != 0) {
        e_calling_title = ber_uid_encode((char *)calling_title.data,
                                    calling_title.size, 0x80);
        calling_t_blen = ber_len_encode(&calling_t_len, 
                                    e_calling_title->size, 4);
        pdusize++;
    }

    if(called_title.size != 0) {
        e_called_title = ber_uid_encode((char *)called_title.data,
                                    called_title.size, 0x80);
        called_t_blen = ber_len_encode(&called_t_len, 
                                e_called_title->size, 4);
        pdusize++;
    }

    if(called_id.size != 0){
        e_called_id = ber_uid_encode((char *)called_id.data,
                                called_id.size, 0x02);
        called_id_blen = ber_len_encode(&called_id_len, 
                                e_called_id->size, 4);
        pdusize++;
    }
#ifdef DEBUG
    printf("    [!] finish id/title fields\n");
    printf("    [!] USER INFO LEN : 0x%x cin %lx cid %lx ced %lx\n", 
        usrinfo_len ,calling_t_len, calling_id_len, called_t_len);
#endif
    //add up all length of title/id 
    pdusize += calling_t_len + calling_id_len 
            + called_t_len + called_id_len 
            + calling_t_blen + called_t_blen 
            + called_id_blen + calling_id_blen;

    int pdulen = 0; long pdublen = 0;
    pdublen = ber_len_encode(&pdulen, pdusize, 4);

    pdusize = 1 + pdublen + pdulen;
#ifdef DEBUG
    printf("    [*] Total PDU size is 0x%x\n", pdusize);
#endif
    ///////////////////////////////////////////////////////////////
    //                                                           //
    //          start to build actual raw data (top down)        //
    //                                                           //
    ///////////////////////////////////////////////////////////////

    //now copy into buffer
    int cum_offset = 0;
    raw_data = new uint8_t[pdusize];

    //first header
    raw_data[0] = 0x60;
    //encoded total length copy 
    memcpy(raw_data + 1, &pdulen, pdublen);

    //set offset to next position 
    cum_offset = 1 + pdublen;
    //printf("Before offset %d\n", cum_offset);

    //add ap element to buffer
    //if pointer is null, it won't add anything
    addAPtoRawData(raw_data, 0xA2, called_t_len, 
        called_t_blen, e_called_title, cum_offset);
    addAPtoRawData(raw_data, 0xA4, called_id_len, 
        called_id_blen, e_called_id, cum_offset);
    addAPtoRawData(raw_data, 0xA6, calling_t_len, 
        calling_t_blen, e_calling_title, cum_offset);
    addAPtoRawData(raw_data, 0xA8, calling_id_len, 
        calling_id_blen, e_calling_id, cum_offset);

    //adding user info section
    raw_data[cum_offset++] = 0xBE;
    memcpy(raw_data + cum_offset, &ele_len, eblen);
    cum_offset += eblen;

    raw_data[cum_offset++] = 0x28;
    memcpy(raw_data + cum_offset, &ext_len, ublen);
    cum_offset += ublen;

    raw_data[cum_offset++] = 0x81;
    memcpy(raw_data + cum_offset, &str_len, str_blen);
    cum_offset += str_blen;

    //copy userinfo data
    memcpy(raw_data + cum_offset, userinfo.data, userinfo.size);
    cum_offset += userinfo.size;
    
#ifdef DEBUG
    printf("    [!] Final acse\n");
    for(int i = 0; i < cum_offset; i++)
        printf("0x%02x ", raw_data[i]);
    puts("");
#endif
    //free up memory....... better manangement needs
    //TO do

    cleanup(e_called_title, "called title");
    cleanup(e_called_id, "called id");
    cleanup(e_calling_title, "calling title");
    cleanup(e_calling_id, "calling id");
#ifdef DEBUG
    printf("    [!!] Finish clean up memory\n");
#endif
    this->acse_len = pdusize;

    return raw_data;
}

/* ================================================================ *
 *                                                                  *
 *                         Parse Helpers                            *
 *                                                                  *
 * ================================================================ */

/**
 * Parse ap element 
 *
 * @param data pointer that start of ap element
 * @param datalen ap element length (outbound)
 * @param berlen encoded length of size (outbound)
 *
 * @return ap element data 
 */
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

/**
 * Parse user information 
 *
 * @param data pointer that start of user information
 * @param datalen user infomation length (outbound)
 * @param berlen size of encoded length (outbound)
 *
 * @return ptr pointer that start of req/res section
 */
uint8_t *
usrinfo_parse(void * data, long * datalen, long * berlen)
{
    uint8_t * ptr = (uint8_t *)data;
    int ber_size;

    *datalen = ber_len_decode(ptr + 1, &ber_size);
    *berlen = ber_size;

    ptr = ptr + 1 + ber_size; //point user_info_external
#ifdef DEBUG
    printf("    [!] userinfo element size : 0x%lx\n", *datalen);
#endif
    //check header for user-info external
    if(*ptr == 0x28)
    {
        *datalen = ber_len_decode(ptr+1, &ber_size);
        ptr = ptr + 1 + ber_size;
#ifdef DEBUG
        printf("    [-] external size : 0x%lx\n", *datalen);
#endif
    }

    //check header for user info octet string
    if(*ptr == 0x81)
    {
        *datalen = ber_len_decode(ptr+1, &ber_size);
        ptr = ptr + 1 + ber_size;
#ifdef DEBUG
        printf("    [-] octet size : 0x%lx\n", *datalen);
#endif
    }

    //now ptr points epsem section
    return ptr;
}

/* ================================================================ *
 *                                                                  *
 *                         Parse Helpers end                        *
 *                                                                  *
 * ================================================================ */

/**
 * Parse ACSE data 
 * 
 * @param data ACSE data 
 */
void
C1222_ACSE::parse(void * data)
{

    uint8_t * ptr = (uint8_t *)data;
    uint8_t * calling_at, *calling_id;
    uint8_t * called_at, *called_id;
    uint8_t * usrinfo;
    int datalen, ber_size;
    int pdusize = 0;

#ifdef DEBUG
    printf("[*] ACSE parsing start..\n");
#endif 
    if(*ptr != 0x60){
#ifdef DEBUG
        printf("[!] Error packet\n");
#endif
        this->error = -1;
        return;
    }
    else {
        datalen = ber_len_decode(ptr + 1, &ber_size);
        ptr = ptr + 1 + ber_size; //now pointing elements
        
#ifdef DEBUG
        printf("    [-] ACSE data length is : %x \n", datalen);
#endif
        long datalen, berlen;
        
        pdusize = 1 + ber_size;
        //parse called
        if(*ptr == 0xA2){
            this->called_title.data = 
                    ap_title_parse(ptr, &datalen, &berlen);
            this->called_title.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
            pdusize += 1 + berlen + datalen;
#ifdef DEBUG
            printf("    [-] Called ap title is : %s\n", 
                this->called_title.data);
#endif
        }

        //check first
        if(*ptr == 0xA4){
            this->called_id.data = 
                    ap_title_parse(ptr, &datalen, &berlen);
            this->called_id.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
            pdusize += 1 + berlen + datalen;
#ifdef DEBUG
            printf("    [-] Called ap invocation id is : %s\n", 
                this->called_id.data);
#endif
        }
        
        //parse calling
        if(*ptr == 0xA6){
            this->calling_title.data = 
                    ap_title_parse(ptr, &datalen, &berlen);
            this->calling_title.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
            pdusize += 1 + berlen + datalen;
#ifdef DEBUG
            printf("    [-] Calling ap title is : %s\n", 
                this->calling_title.data);
#endif
        }
         
        if(*ptr == 0xA8){
            this->calling_id.data = 
                    ap_title_parse(ptr, &datalen, &berlen);
            this->calling_id.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
            pdusize += 1 + berlen + datalen;
#ifdef DEBUG
            printf("    [-] Calling ap invocation id is : %s\n", 
                this->calling_id.data);
#endif
        }
        
        //if calling id is not present, stop process 
        if(this->calling_id.size == 0 &&
                this->calling_id.data == NULL)
        {
#ifdef DEBUG
            printf("    [!] Calling id is not present, abort parsing\n");
#endif
            this->error = -2;
            return;
        }
      
        //parse user information
        if(*ptr == 0xBE){
            //this->userinfo.data = ptr;
            this->userinfo.data = usrinfo_parse(ptr, &datalen, &berlen);
            this->userinfo.size = datalen;
            ptr = ptr + 1 + berlen + datalen;
#ifdef DEBUG
            printf("    [-] userinfo data start with\
: 0x%x and length 0x%lx\n", *(this->userinfo.data), datalen);
#endif 
            pdusize += 1 + berlen + datalen;
        } else {
#ifdef DEBUG
            printf("    [!] User info Header is not present, abort\n");
            this->error = -3;
            return;
#endif
        }
        
        this->acse_len = pdusize;
    }
#ifdef DEBUG
    printf("[*] ACSE Parsing done!\n");
#endif
}

/* ================================================================ *
 *                                                                  *
 *                         Setter/Getter                            *
 *                                                                  *
 * ================================================================ */

/**
 *
 *
 */ 
char * 
C1222_ACSE::get_calling_title()
{
    return (char *)this->calling_title.data;
}

/**
 *
 *
 */ 
char * 
C1222_ACSE::get_calling_id()
{
    return (char *)this->calling_id.data;;
}
 
/**
 *
 *
 */     
char * 
C1222_ACSE::get_called_title()
{
    return (char *)this->called_title.data;;
}
      
/**
 *
 *
 */           
char * 
C1222_ACSE::get_called_id()
{
    return (char *)this->called_id.data;;
}
 
/**
 *
 *
 */ 
uint8_t * 
C1222_ACSE::get_epsem()
{
    return this->userinfo.data;
}

/**
 *
 *
 */ 
long
C1222_ACSE::get_build_size()
{
    return this->acse_len;
}

int
C1222_ACSE::get_error()
{
    return this->error;
}
/**
 *
 *
 */ 
void
C1222_ACSE::set_epsem(void * data, long size)
{
    
}


