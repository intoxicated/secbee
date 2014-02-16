/**
 * Utility template functions 
 *
 * Wooyoung Chung
 */

#ifndef __util_h__
#define __util_h__

template <int a>
int ber_len_size(int n);

template <int a>
int ber_encode(void * ptr, int len, int mx);

template <int a>
int ber_decode(void * ptr, void * buffer);

template <int a>
int checksum(void * ptr, int offset, int len);

/**
 * Calculate length to be used in BER 
 * 
 * @param length
 * @return BER length 
 */
template <int a>
int ber_len_size(int n)
{

    return 0;
}


/**
 * @param ptr buffer to be used for result encoding
 * @param len length used by bar
 * @param mx max 
 *
 * @return 
 */
template <int a>
int ber_encode(void * ptr, int len, int mx)
{
    return 0;
}

/**
 * @param ptr bar encoded buffer
 * @param buffer result buffer
 *
 * @return 
 */
template <int a>
int ber_decode(void * ptr, void * buffer)
{
    return 0;
}

/**
 * @param pointer to buffer containing data 
 * @param offset beginning of data
 * @param length of data to be checked
 *
 * @return check sum value
 */
template <int a>
int checksum(void * ptr, int offset, int len)
{
    int sum = 0;
    for(int i = 0; i < len; i++)
        sum += ptr[offset++];
    return (((sum - 1) & 0xFF) ^ 0xFF);
}

#endif
