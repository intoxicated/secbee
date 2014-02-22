/**
 * Utility template functions 
 *
 * Wooyoung Chung
 */

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdint.h>
#include <cstddef>

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
 *
 * Foramt:
 * value1.value2.value3. ... .valuen
 *
 * first byte value 40 x value1 + value2
 * following by - value(i) encoded base 128 most sig digit first and most
 *                significant bit 1 except last byte 
 *
 *
 */

inline int ber_uid_encode(void * ptr, int len)
{

    return 0;
}

/**
 * decode universal identifier (most likely aptitle)
 *
 * 
 */
inline uint8_t * ber_uid_decode(void * data, int& size)
{
    
    return 0;
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
