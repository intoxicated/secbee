/**
 * Utility template functions 
 *
 * Wooyoung Chung
 */

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <cstddef>
#include <stdlib.h>
#include <stdio.h>
#include "C1222.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * Called AP title element
 * 
 * identify the target of ACSE message
 *  
 * -----------------------------------
 * | 0xA2 | title length | ap title  |
 * -----------------------------------
 *
 * ap title (universal aptitle element)
 * -------------------------------------
 * | 0x06 | id length | universal id   |
 * -------------------------------------
 */

/**
 * Calling AP Title element
 *
 * identfiy the source of ACSE message
 *
 * -----------------------------------
 * | 0xA6 | title length | ap title  |
 * -----------------------------------
 *
 * ap title (universal aptitle element)
 * -----------------------------------------------------------
 * | 0x06 | len(byte) | universal id (not exceed 127 bytes)  |
 * -----------------------------------------------------------
 */
//app-context-oid := 06 07 60 7C 86 F7 54 01 16 

    
/**
 * Encode data into Universal identifier 
 * Relative **
 * Foramt:
 * value1.value2.value3. ... .valuen
 *
 * first byte value 40 x value1 + value2
 * following by - value(i) encoded base 128 most sig digit first and most
 *                significant bit 1 except last byte 
 *
 * due to complexity issue, encode in relative manner
 *
 * @param ptr string in "x.x.xx" format
 * @param len length of string
 *
 * @return ap_element struct data (0x80 <length> <encoded data>) with size
 */

inline ap_element * ber_uid_encode(char * ptr, int len)
{
    
}

/**
 * decode universal identifier (most likely aptitle)
 * relative calling aptitle decode 
 *
 * caller must to free the memory after calling this function
 *
 * @param data encoded raw data
 * @param size (outbound) length of byte
 */
inline char * ber_uid_decode(void * data, int * size)
{
    uint8_t * origin = (uint8_t *)data + 1;
    uint8_t * ptr = (uint8_t *)data;

    //save encoded length 
    *size = ptr[0];
    //move pointer to first byte of data
    ptr++;

    //innerCount : flag for how much I need to shift
    //bcount : flag for couting left bit 
    int innerCount = 3, bcount = 0;;

    //temporary holder for value
    unsigned int temp = 0;
    uint8_t val  = 0;

    //max 127 bytes of data (defined in c1222 doc)
    char * ret = (char *)malloc(512);
    memset(ret, 0x0, 512);

    for(;ptr < origin + *size; ++ptr)
    {
        //check for block of value
        //8th bit is not on than last block 
        bcount ++;
        
        //if most significant bit is 0, we know
        //this is last block of a value 
        if(*ptr >> 7 == 0)
        {
            //concat to current temporary holder
            temp |= (*ptr) << ((innerCount-- * 8) + bcount);

            //shift properly to recover original value 
            temp = temp >> (bcount + (innerCount+1) * 8);
            
            //copy to buffer
            snprintf(ret + strlen(ret), sizeof(ret), "%u", temp);
            strcat(ret, ".");
            
            //reset flags
            innerCount = 3;     
            temp = 0;
            bcount = 0;
        }
        else
        {
            //concat 7bit to holder with proper shifting
            temp |= (*ptr & 0x7F) << ((innerCount-- * 8) + bcount);
        }
    }
    
    ret[strlen(ret)-1] = '\0'; //null terminate
    return ret;
}

/**
 * Calculate length to be used in BER 
 * 
 * @param length
 * @return BER length 
 */
inline int ber_len_size(int n)
{
    int len = 1; //byte length
    if(n > 0x7f) len ++;
    if(n > 0xff) len ++;
    if(n > 0xffff) len ++;
    if(n > 0xffffff) len ++;
    return len;
}


/**
 * length field  encoding
 *
 * @param ptr buffer to be used for result encoding
 * @param len length used by bar
 * @param mx max 
 *
 * @return length of ber encoding 
 */
inline int ber_len_encode(void * ptr, int len, int mx)
{
    int berlen = ber_len_size(len);
    int ret = 0; 
    uint8_t * bptr = (uint8_t *)ptr;
    
    if(berlen > mx) //berlen exceed max
        return 0;
    if(berlen == 1) //use number itself
        *bptr = len;
    else
    {
        //number of byte after header
        *bptr = (berlen - 1) | 0x80;
        
        for(int i = berlen - 2; i >= 0; i--)
            *bptr++ = (len >> (i*8)) & 0xFF;
    }

    return berlen;
}

/**
 * length decoding 
 *
 * @param ptr bar encoded buffer
 * @param ber_size (outbound)
 * @return actual length 
 * */
inline int ber_len_decode(void * data, int * size)
{
    uint8_t * bptr = (uint8_t *)data;
    
    int ret = 0; 
    
    if(*bptr <= 0x7f){
        ret |= *bptr;
        *size = 1;
    }
    else
    {
        *size = *bptr & 0x0F;
        
        bptr += 1;

        for(int i = *size-1; i >= 0; i--) {
            ret |= (0xFF & *bptr++) << (i * 8);
        }
    }
    
    return ret;
}

/**
 * @param pointer to buffer containing data 
 * @param offset beginning of data
 * @param length of data to be checked
 *
 * @return check sum value
 */
/*
int checksum(void * ptr, int offset, int len)
{
    int sum = 0;
    for(int i = 0; i < len; i++)
        sum += ptr[offset++];
    return (((sum - 1) & 0xFF) ^ 0xFF);
}
*/
inline void copy_to(int * dst, void * source, int len)
{
    uint8_t * ptr = (uint8_t *)source;
    for(int i = len - 1; i >= 0; i--)
        *dst |= (*ptr++) << (i * 8);
}

#ifdef __cplusplus
}
#endif

#endif
