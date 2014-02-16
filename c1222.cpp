#include "c1222.h"
#include "Utils.hpp"

#include <iostream>

/* ======================================================================== *
 *                                                                          *
 *                            C1222 packet class                            *  
 *                                                                          *
 * ======================================================================== */


/**
 * packet methods
 */
c1222_packet::c1222_packet(char * called_ap, char * called_id,
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
c1222_packet& 
parse(void * data)
{
    byte * ptr = (byte *)data;
    if(ptr[0] != 0x60)
        std::domain_error("invalid packet header");


}

/**
 * build c1222 packet and return pointer
 *
 */ 
void *
c1222_packet::build()
{


}



/* ========================================================================= *
 *                                                                           *
 *                                 ACSE class                                *  
 *                                                                           *
 * ========================================================================= */


/* ========================================================================= *
 *                                                                           *
 *                                EPSEM class                                *  
 *                                                                           *
 * ========================================================================= */


/* ========================================================================= *
 *                                                                           *
 *                               Request class                               *  
 *                                                                           *
 * ========================================================================= */

/** 
 * return request raw data 
 */
void *
c1222_request::build()
{
    return raw;
}

/**
 * free allocated memory for raw array
 * this function has to be called after usage of built data
 * in order to prevent memory leak
 */
void
c1222_request::clear()
{
    delete raw;
}

/**
 *  Identity request 
 *  Detail : Obtain device functionality 
 *  Header : 0x20
 */
void
c1222_request::identify()
{
    this->raw = new byte[1];
    this->request_num = '\x20';

    this->raw[0] = this->request_num;
}

/**
 * Read request
 * Detail : Transfer of table data from target
 * Header : 0x30         complete table
 *          0x31 - 0x39  partial table using 1-9 indices
 *          0x3E         default able
 *          0x3F         partial table using offset/octcount 
 * 
 * @param tableid   byte array that contains table id (2bytes)
 * @param offset    offset of table (only for partial read 0x3F)
 * @param octcount  octet count number (only for parital read 0x3F)
 */
void 
c1222_request::read(const byte* tableid, const byte* offset, 
        const byte* octcount)
{
    this->request_num = '\x30';
    byte  table_id[] = "\x00\x01"; //should use paramter later
   
    //offset and octcount should be used for other 
    //than full read (i.e partial read)
    //
    this->raw = new byte[3]; //for now full read 

}

/**
 * Write request
 * Detail : Transfer of table data to target
 * Header : 0x40        complete table 
 *          0x41 - 0x49 partial table using 1-9 indices
 *          0x4F        partial table using offset/octcount 
 *          
 * @param tableid   byte array that contains table id (2bytes)
 * @param offset    offset of table (only for partial write 0x4F)
 * @param octcount  octet count number (only for partial write 0x4F)
 * @param data      typically byte array of table data 
 */
void
c1222_request::write(const byte * tableid, const byte * offset, 
        const byte * octcount, void * data)
{

}

/**
 * Logon request
 * Detail : establish a session without permission 
 * Note   : userid may be logged in target device if C1222 node 
 * Header : 0x50
 *
 * @param user_id_code     user identification code  (word16)
 * @aaram id               user identification (10 bytes)
 * @param timeout          request session idle timeout (0 is forever) (word16)
 */
void
c1222_request::logon(const byte * user_id_code, const byte * id, 
        const byte * timeout)
{

}

/**
 * Security request
 * Detail : provide security to change table access privilege or auth
 * Note   : this request is not security mechanism for c1222 protocol 
 * Header : 0x51
 *
 * @param passwd        password (20bytes fixed)
 * @param user_id_code  user identification code
 */
void 
c1222_request::security(const byte * passwd, const byte * user_id_code)
{

}

/**
 * Logoff request
 * Detail : termination of the session that was established by logon req
 * Header : 0x52
 */
void
c1222_request::logoff(void)
{

}

/**
 * Terminate request
 * Detail : abortion of the session that was established by logon req
 *          used for external/intenral error, security issues, and 
 *          any other reasons 
 * Header : 0x21
 */
void
c1222_request::terminate(void)
{

}

/**
 * Disconnect request
 * Detail : remove c1222 node from c1222 network 
 *          in order to execute this, initiator must have write access
 *          permission to Procedure 25 NETWORK_INTERFACE_CONTROL_PROC
 * Header : 0x22
 */
void
c1222_request::disconnect(void)
{

}

/** 
 * Wait request
 * Detail : maintain an established session during idle period 
 * Header : 0x70
 *
 * @param interval wait period in seconds 
 */
void
c1222_request::wait(const byte interval)
{

}

/**
 * Registration request
 * Detail : add and keep routing table entries of c1222 relays active
 * Note   : to be part of c1222 network, node must send request to one 
 *          of c1222 master relays. * this request is carried as following
 *
 *          <ACSE-PDU> with
 *          <calling ap title> ap title of registering node
 *          <called ap title> ap title of master relay
 *          node type, node class, serial number, reg lifetime params
 *          optional native address of registering node (only use for 
 *          neighbor relay to send message back to this node) 
 *
 *          Master relay sends a copy of registration to all notification
 *          hosts and all c1222 auth hosts that need to be notified 
 * Header : 0x27
 *
 * @param node_type identification of node's attribute (byte)
 *                  bit 0 : relay
 *                  bit 1 : master relay
 *                  bit 2 : host
 *                  bit 3 : notification host
 *                  bit 4 : auth host
 *                  bit 5 : end device (1 is 1219 device, 0 is 1222)
 *
 * @param connection_type type of connection (byte)
 *                  bit 0 : broadcast_and_multicast_supported
 *                  bit 1 : message_acceptance_window_supported
 *                  bit 2 : playback_rejection_supported (filter dup)
 *                  bit 3 : reserve (0)
 *                  bit 4 : connectionless_mode_supported
 *                  bit 5 : accept_connectionless (related 4)
 *                  bit 6 : connection_mode_support (should 1 if 4 is off)
 *                  bit 7 : accept_connection (related 6)
 *
 * @param device_class manufacturer_id in table 00 or device_class (4 bytes)
 * @param ap_title      aptile of node (optional)
 * @param serial_num    unique serial number of device (optional) (table 122)
 * @param addr_len      number of byte in native addr
 * @param native_addr   native address to use to foward message (optional
 *                      if lower layer provides it)
 * @param reg_period    max period in seconds desired by node to elapse 
 *                      between successful re-reg request (default 0) (word24)
 */
void
c1222_request::registration(const byte node_type, const byte conn_type,
        const byte * device_class, const byte * ap_title, 
        const byte * serial_num, const byte addr_len, const byte * native_addr,
        const byte * reg_period)
{

}

/**
 * Deregistration request
 * Detail : remove routing table entries of c1222 relay, master relay 
 *          and all notification hosts
 *
 * Header : 0x24
 *
 * @param ap_title aptitle of deregister device 
 */
void
c1222_request::deregistration(const byte * ap_title)
{

}

/**
 * Resolve request
 * Detail : retrieve the native address of c1222 node, this address is used
 *          to communicate directly with other c1222 node in same network
 * Note   : this also can be used to retrieve list of addresses relay by 
 *          not providing called ap title and calling ap title (optional)
 * Header : 0x25
 *
 * @param ap_title aptitle of requested node 
 */
void
c1222_request::resolve(const byte * ap_title)
{

}

/**
 * Trace request
 * Detail : retrieve the list of c1222 relays that have forwarded c1222 
 *          message to a target node. 
 * Note   : each time relay receives this request, it adds its own aptitle
 *          to the list in User information element and forward it to next
 *          When it reaches target node, it includes its aptitle and send 
 *          ok response with it to requesting node. 
 * Header : 0x26
 *
 * @param aptitle called aptitle (target)
 */
void
c1222_request::trace(const byte * ap_title)
{

}
