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


C1222_packet& 
parse(void * data)
{
    uint8_t * ptr = (uint8_t *)data;

}

/**
 * build C1222 packet and return pointer
 *
 */ 
void *
C1222_packet::build()
{


}
