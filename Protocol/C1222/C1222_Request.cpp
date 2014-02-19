/* ========================================================================= *
 *                                                                           *
 *                        C1222 Request Implementation                       *  
 *                                                                           *
 * ========================================================================= */


#include "C1222_Request.h"
#include <sstream>

using namespace std;

/** 
 * return request raw data 
 */
void *
C1222_Request::build()
{
    return raw;
}

/**
 * free allocated memory for raw array
 * this function has to be called after usage of built data
 * in order to prevent memory leak
 */
void
C1222_Request::clear()
{
    delete raw;
}

/**
 * Identity request 
 * Detail : Obtain device functionality 
 * Header : 0x20
 */
void
C1222_Request::identify()
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
<<<<<<< HEAD
C1222_Request::read(const byte* tableid, const byte* offset, 
        const byte* octcount)
=======
C1222_request::read(const byte * tableid, const byte * offset, 
        const byte * octcount)
>>>>>>> 1f3f87e46e76f3e6ebb4b9602e00f386321ef52e
{
    this->request_num = '\x30';
    byte  table_id[] = "\x00\x01"; //should use paramter later
   
    //offset and octcount should be used for other 
    //than full read (i.e partial read)
    
    this->raw = new byte[3]; //for now full read 
<<<<<<< HEAD
=======
    this->raw[0] = this->request_num;
    memcpy(raw+1, table_id, 2);

>>>>>>> 1f3f87e46e76f3e6ebb4b9602e00f386321ef52e
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
C1222_Request::write(const byte * tableid, const byte * offset, 
        const byte * octcount, void * data)
{

}

/**
 * Logon request
 * Detail : establish a session without permission 
 * Note   : userid may be logged in target device if C1222 node 
 * Header : 0x50
 *
 * @param user_id          user identification code  (2 bytes)
 * @aaram usernmae         user identification (10 bytes)
 * @param timeout          request session idle timeout (0 is forever) (2 bytes)
 */
void
C1222_Request::logon(const unsigned short user_id, const string username, 
        const unsigned short timeout)
{
    char uname[10];
    this->request_num = '\x50';
    this->raw = new byte[15];

    //validation of input
    if(username.length() > 10)
        std::out_of_range("Request(Logon): out of range");

    //add header
    this->raw[0] = this->request_num;
    
    //add userid
    std::stringstream sstream;
    sstream << std::hex << user_id;
    std::string ustr = sstream.str();
    memcpy(this->raw + 1, ustr.c_str(), 2);

    //padding username to 10 bytes 
    memcpy(uname, username.c_str(), username.length());
    for(int i = 0; i < (10 - username.length()); i++)
        memcpy(uname+i, "\x20", 1);
    //add username
    memcpy(this->raw + 3, uname, 10);

    //add timeout
    sstream << std::hex << timeout;
    std::string tstr = sstream.str();
    memcpy(this->raw + 13, tstr.c_str(), 2);
}

/**
 * Security request
 * Detail : provide security to change table access privilege or auth
 * Note   : this request is not security mechanism for C1222 protocol 
 * Header : 0x51
 *
 * @param passwd        password (20bytes fixed)
 * @param user_id_code  user identification code
 */
void 
C1222_Request::security(const string passwd, const unsigned short user_id)
{
    if(passwd.length() > 20)
        std::out_of_range("Request(security): passwd out of range");

    char pw[20];

    this->request_num = '\x51';
    this->raw = new byte[23];

    //add header
    this->raw[0] = this->request_num;

    //add password
    memcpy(pw, passwd.c_str(), passwd.length());
    for(int i = 0; i < (20 - passwd.length()); i++)
        memcpy(pw+i, "\x20", 1);
   
    //add userid
    std::stringstream sstream;
    sstream << std::hex << user_id;
    std::string ustr = sstream.str();
    memcpy(this->raw + 21, ustr.c_str(), 2);
}

/**
 * Logoff request
 * Detail : termination of the session that was established by logon req
 * Header : 0x52
 */
void
C1222_Request::logoff(void)
{
    this->raw = new byte[1];
<<<<<<< HEAD
    this->request_num = '\x52';
    
    this->raw[0] = this->request_num;
=======
    this->raw[0] = '\x52';
>>>>>>> 1f3f87e46e76f3e6ebb4b9602e00f386321ef52e
}

/**
 * Terminate request
 * Detail : abortion of the session that was established by logon req
 *          used for external/intenral error, security issues, and 
 *          any other reasons 
 * Header : 0x21
 */
void
C1222_Request::terminate(void)
{
    this->raw = new byte[1];
    this->request_num = '\x21';
    
    this->raw[0] = this->request_num;
}

/**
 * Disconnect request
 * Detail : remove C1222 node from C1222 network 
 *          in order to execute this, initiator must have write access
 *          permission to Procedure 25 NETWORK_INTERFACE_CONTROL_PROC
 * Header : 0x22
 */
void
C1222_Request::disconnect(void)
{
    this->raw = new byte[1];
<<<<<<< HEAD
    this->request_num = '\x22';
    
    this->raw[0] = this->request_num;
=======
    this->raw[0] = '\x22';
>>>>>>> 1f3f87e46e76f3e6ebb4b9602e00f386321ef52e
}

/** 
 * Wait request
 * Detail : maintain an established session during idle period 
 * Header : 0x70
 *
 * @param interval wait period in seconds 
 */
void
C1222_Request::wait(const byte interval)
{
    this->raw = new byte[2];
<<<<<<< HEAD
    this->request_num = '\x70';
    
    this->raw[0] = this->request_num;
    this->raw[1] = interval;
=======
    this->raw[0] = '\x70';
    this->raw[1] = internal;
>>>>>>> 1f3f87e46e76f3e6ebb4b9602e00f386321ef52e
}

/**
 * Registration request
 * Detail : add and keep routing table entries of C1222 relays active
 * Note   : to be part of C1222 network, node must send request to one 
 *          of C1222 master relays. * this request is carried as following
 *
 *          <ACSE-PDU> with
 *          <calling ap title> ap title of registering node
 *          <called ap title> ap title of master relay
 *          node type, node class, serial number, reg lifetime params
 *          optional native address of registering node (only use for 
 *          neighbor relay to send message back to this node) 
 *
 *          Master relay sends a copy of registration to all notification
 *          hosts and all C1222 auth hosts that need to be notified 
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
C1222_Request::registration(const byte node_type, const byte conn_type,
        const byte * device_class, const byte * ap_title, 
        const byte * serial_num, const byte addr_len, const byte * native_addr,
        const byte * reg_period)
{

}

/**
 * Deregistration request
 * Detail : remove routing table entries of C1222 relay, master relay 
 *          and all notification hosts
 *
 * Header : 0x24
 *
 * @param ap_title aptitle of deregister device 
 */
void
C1222_Request::deregistration(const byte * ap_title)
{

}

/**
 * Resolve request
 * Detail : retrieve the native address of C1222 node, this address is used
 *          to communicate directly with other C1222 node in same network
 * Note   : this also can be used to retrieve list of addresses relay by 
 *          not providing called ap title and calling ap title (optional)
 * Header : 0x25
 *
 * @param ap_title aptitle of requested node 
 */
void
C1222_Request::resolve(const byte * ap_title)
{

}

/**
 * Trace request
 * Detail : retrieve the list of C1222 relays that have forwarded C1222 
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
C1222_Request::trace(const string ap_title)
{
<<<<<<< HEAD
    this->raw = new byte[1+ap_title.length()];
    this->request_num = '\x26';
    
    this->raw[0] = this->request_num;
    memcpy(this->raw + 1, ap_title.c_str(), ap_title.length());
=======

>>>>>>> 1f3f87e46e76f3e6ebb4b9602e00f386321ef52e
}
