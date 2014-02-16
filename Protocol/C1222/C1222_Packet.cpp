/* ======================================================================== *
 *                                                                          *
 *                          C1222 packet Implementation                     *  
 *                                                                          *
 * ======================================================================== */

#include "C1222_packet.h"
/**
 * packet methods
 */
C1222_packet::C1222_packet(char * called_ap, char * called_id,
        char * calling_ap, char * calling_id, void * data, int len)
{
    this->called_ap = called_ap;
    this->called_ap_invo = called_id;
    this->calling_ap = calling_ap;
    this->calling_ap_invo = calling_id;

    this->data = data;
    this->length = len;
}

/**
 * minimal packet format overview
 *
 * ACSE PDU
 * ---------------------------
 * | 0x60 | length | element |
 * ---------------------------
 *
 * element                       if segment
 * -----------------------------  ---------------
 * | calling-ap-id | user-info |  seg user info |
 * -----------------------------  ---------------
 *
 * calling-ap-id
 * ----------------------------------------------------------------------
 * | 0xA8 | int length (bytes) | 0x02 | id length (bytes) | id (bytes)* | 
 * ----------------------------------------------------------------------
 *
 * user-info
 * ----------------------------------------------------
 * | 0xBE | ext len (bytes) | 0x28 | info len (bytes) |
 * ----------------------------------------------------
 * | 0x81 | oct str len (bytes) | epsem               |
 * ----------------------------------------------------
 *
 * epsem
 * ----------------------------------------------------
 * | epsem ctrl (8bits)| serv len (bytes | req / res  | 
 * ----------------------------------------------------
 *
 */
C1222_packet& 
parse(void * data)
{
    byte * ptr = (byte *)data;
    if(ptr[0] != 0x60)
        std::domain_error("invalid packet header");


}

/**
 * build C1222 packet and return pointer
 *
 */ 
void *
C1222_packet::build()
{


}