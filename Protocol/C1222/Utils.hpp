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
 * | 0x80 | id length | universal id   |
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
 * | 0x80 | len(byte) | universal id (not exceed 127 bytes)  |
 * -----------------------------------------------------------
 */

/**
 * Encode data into Universal identifier 
 * Relative **
 *
 * due to complexity issue, encode in relative manner
 * caller responsible to free data in struct and struct itself
 *
 * @param ptr string in "x.x.xx" format
 * @param len length of string
 *
 * @return element struct data (0x80 <length> <encoded data>) with size
 */

inline element * ber_uid_encode(char * ptr, int len, int tag)
{
    char * idPtr[512];
    int count = 0;
    
    char dup[len+1];
    strcpy(dup, ptr);
    dup[len] = '\0';

    idPtr[count] = strtok(dup, ".");

    //store pointer to each value
    while(idPtr[count] != NULL)
    {
        count++;
        idPtr[count] = strtok(NULL, ".");
    }

    uint8_t * tempVals[count]; 
    unsigned int sizes[count];
    unsigned int temp, intVal;

    uint8_t mask = 0x80;
    int totalSize = 0;    
    for(int i =0; i< count; i++)
    {
        intVal = atoi(idPtr[i]);
        //for each value

        tempVals[i] = new uint8_t[5];
        temp = (intVal >> 28) & 0x0F; //last 4 bits

        //TODO: maybe better way to parse this
        if(temp != 0){
            tempVals[i][0] = mask | temp;
            tempVals[i][1] = mask | ((intVal >> 21) & 0x7F);
            tempVals[i][2] = mask | ((intVal >> 14) & 0x7F);
            tempVals[i][3] = mask | ((intVal >> 7 ) & 0x7F);
            tempVals[i][4] = (intVal & 0x7F);
            sizes[i] = 5;
        }
        else if((temp = ((intVal >> 21) & 0x7F)) != 0)
        {
            tempVals[i][0] = mask | temp;
            tempVals[i][1] = mask | ((intVal >> 14) & 0x7F);
            tempVals[i][2] = mask | ((intVal >> 7 ) & 0x7F);
            tempVals[i][3] = (intVal & 0x7F);
            sizes[i] = 4;
        } 
        else if((temp = ((intVal >> 14) & 0x7F)) != 0)
        {
            tempVals[i][0] = mask | temp;
            tempVals[i][1] = mask | ((intVal >> 7) & 0x7F);
            tempVals[i][2] = (intVal & 0x7F);
            sizes[i] = 3;
        }
        else if((temp = ((intVal >> 7) & 0x7F)) != 0)
        {
            tempVals[i][0] = mask | temp;
            tempVals[i][1] = (intVal & 0x7F);
            sizes[i] = 2;
        }
        else{
            tempVals[i][0] = (intVal & 0x7F);
            sizes[i] = 1;
        }

        totalSize += sizes[i];
    }

    element * ret = new element();
    
    ret->size = totalSize + 2;
    ret->data = new uint8_t[totalSize + 2];

    //header and size
    ret->data[0] = tag;
    ret->data[1] = totalSize;
    //totalSize cannot exceed 127 bytes
    for(int i = 0; i < count; i++)
    {
        if(i == 0)
            memcpy(ret->data + 2, tempVals[i], sizes[i]);
        else 
            memcpy(ret->data + 2 + (i*sizes[i-1]), tempVals[i], sizes[i]);
        free(tempVals[i]);
    }
    
    return ret;
}

/**
 * decode universal identifier (most likely aptitle)
 * relative calling aptitle decode 
 *
 * caller must to free the memory after calling this function
 *
 * @param data encoded raw data starting with length of data
 * @param size (outbound) length of byte
 */
inline char * ber_uid_decode(void * data)
{
    uint8_t * origin = (uint8_t *)data + 1;
    uint8_t * ptr = (uint8_t *)data ;

    //save encoded length 
    int size = ptr[0];


    //move pointer to first byte of data
    ptr++;
    //innerCount : flag for how much I need to shift
    //bcount : flag for couting left bit 
    int innerCount = 3, bcount = 0;;
    //temporary holder for value
    unsigned int temp = 0;
    uint8_t val  = 0;

    //max 127 bytes of data (defined in c1222 doc)
    char * ret = new char[512];
    memset(ret, 0x0, 512);

    for(;ptr < origin + size; ++ptr)
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
