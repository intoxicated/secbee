/* ========================================================================= *
 *                                                                           *
 *                        C1222 Request Implementation                       *  
 *                                                                           *
 * ========================================================================= */


#include "C1222_Request.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

/**
 *  Super class Request
 *
 */

C1222_Request::C1222_Request()
{
    build_size = 0;
    raw_data = NULL;
    request_num = 0;
}

C1222_Request::~C1222_Request()
{
    if(build_size > 0 && raw_data != NULL)
        delete raw_data;
}

uint8_t *
C1222_Request::build()
{
    return raw_data;
}

uint8_t
C1222_Request::get_request_num()
{
    return request_num;
}

unsigned long
C1222_Request::get_build_size()
{
    return build_size;
}

/**
 * free allocated memory for raw array
 * this function has to be called after usage of built data
 * in order to prevent memory leak
 */

/**
 * Identity request 
 * Detail : Obtain device functionality 
 * Header : 0x20
 */
C1222_Request_Ident::C1222_Request_Ident():C1222_Request()
{
    request_num = 0x20;
    build_size = 0;
}

uint8_t *
C1222_Request_Ident::build()
{
    raw_data = new uint8_t[1];
    raw_data[0] = request_num;
    build_size = 1;

    return raw_data;
}


/**
 * Logoff request
 * Detail : termination of the session that was established by logon req
 * Header : 0x52
 */
C1222_Request_Logoff::C1222_Request_Logoff():C1222_Request()
{
    this->request_num = 0x52;
    build_size = 0;
}

uint8_t *
C1222_Request_Logoff::build()
{
    this->raw_data = new uint8_t[1];
    this->raw_data[0] = this->request_num;
    build_size = 1;

    return raw_data;
}


/**
 * Disconnect request
 * Detail : remove C1222 node from C1222 network 
 *          in order to execute this, initiator must have write access
 *          permission to Procedure 25 NETWORK_INTERFACE_CONTROL_PROC
 * Header : 0x22
 */
C1222_Request_Disconnect::C1222_Request_Disconnect():C1222_Request()
{
    this->request_num = 0x22;
    build_size = 0;
}

uint8_t *
C1222_Request_Disconnect::build()
{
    this->raw_data = new uint8_t[1];
    this->raw_data[0] = this->request_num;
    build_size = 1;

    return raw_data;
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
C1222_Request_Logon::C1222_Request_Logon():C1222_Request()
{
    request_num = 0x50;
    user_id = 0;
    timeout = 0;
    build_size = 0;
}

C1222_Request_Logon::C1222_Request_Logon(unsigned short user_id, 
                                          const char * username, 
                                         unsigned short timeout)
{
    request_num = 0x50;
    this->user_id = user_id;
    this->timeout = timeout;
    build_size = 0;
    strcpy(this->username, username);
}

unsigned short
C1222_Request_Logon::get_user_id()
{
    return user_id;
}
    
unsigned short 
C1222_Request_Logon::get_timeout()
{
    return timeout;
}

char *         
C1222_Request_Logon::get_username()
{
    return username;
}

uint8_t *
C1222_Request_Logon::build()
{
    unsigned short uid, tout;

    raw_data = new uint8_t[15];

    //validation of input
    //if(strlen(username) > 10)
    //    throw std::out_of_range("Request(Logon): out of range");

    //add header
    raw_data[0] = this->request_num;
    
    //add userid //need to change in c style
    //std::stringstream sstream;
    //sstream << std::hex << user_id;
    //std::string ustr = sstream.str();
    uid = user_id >> 8 | user_id << 8;
    tout = timeout >> 8 | timeout << 8;
    
    memcpy(raw_data + 1, &uid, 2);

    char uname[10];
    memcpy(uname, username, strlen(username));
    //padding username to 10 byte 
    int usrlen = strlen(username);
    if(usrlen < 10)
        memset(uname + usrlen, 0x20, 10 - usrlen);
 
    //add username
    memcpy(raw_data + 3, uname, 10);

    //add timeout
    memcpy(raw_data + 13, &tout, 2);

    build_size = 15;

    return raw_data;
}

C1222_Request_Logon 
logon_parse(uint8_t * data)
{
    uint8_t req;
    unsigned short userid, timeout;
    char username[11];

    req = *data; //first byte is header
    
    memcpy(&userid, data + 1, 2); //byte swap?
    memcpy(username, data + 3, 10);
    memcpy(&timeout, data + 13, 2);
    username[10] = '\0';

    C1222_Request_Logon logon(userid, username, timeout);

    return logon;
}


/**
 * Read request
 * Detail : Transfer of table data from target
 * Header : 0x30         complete table
 *          0x31 - 0x39  partial table using 1-9 indices
 *          0x3E         default able
 *          0x3F         partial table using offset/octcount 
 * 
 * @param tableid   uint8_t array that contains table id (2uint8_ts)
 * @param offset    offset of table (only for partial read 0x3F)
 * @param octcount  octet count number (only for parital read 0x3F)
 */

C1222_Request_Read::C1222_Request_Read():C1222_Request()
{
    request_num = 0x30;
    tableid = 0;
    e_count = 0;
    index = NULL;
    index_size = 0;
    build_size = 0;
}

C1222_Request_Read::~C1222_Request_Read()
{
    if(index_size > 0)
        delete this->index;
}

C1222_Request_Read::C1222_Request_Read(uint8_t req, unsigned short tableid,
                                 uint8_t * index, unsigned short index_size, 
                                 unsigned short e_count)
{
    request_num = req;
    this->tableid = tableid;
    this->e_count = e_count;
    this->index_size = index_size;
    build_size = 0;

    if(index_size > 0){
        this->index = new uint8_t[index_size];
        memcpy(this->index, index, index_size);
    }
}

uint8_t * 
C1222_Request_Read::get_index()
{
    return index;
}

unsigned short
C1222_Request_Read::get_tableid()
{
    return tableid;
}

unsigned short
C1222_Request_Read::get_e_count()
{
    return e_count;
}

uint8_t *
C1222_Request_Read::build()
{
    //full read
    if(0x30 == request_num)
    {
        build_size = 3;
        raw_data = new uint8_t[build_size];
        raw_data[0] = request_num;
        memcpy(raw_data + 1, &tableid, 2);
       
    }
    //partial index
    else if(0x30 < request_num && 0x39 >= request_num)
    {
        //TODO
    }
    //default
    else if(0x3E == request_num)
    {
        build_size = 1;
        raw_data = new uint8_t[build_size];
        raw_data[0] = request_num;
    }
    //partial offset
    else if(0x3F == request_num)
    {
        //TODO
    }
    else
        ; //error

    return raw_data;
}

C1222_Request_Read 
read_parse(uint8_t * data)
{
    uint8_t req = *data;
    unsigned short tableid = 0, size = 0, e_count = 0;
    //full
    if(0x30 == req)
    {
        memcpy(&tableid, data, 2);
    }
    //default
    else if (0x3E == req)
    {
        //nothing
    }
    //partial index
    else if (0x30 < req && 0x39 >= req)
    {
        memcpy(&tableid, data, 2);
        //index size???
    }
    else if (0x3F == req)
    {
        //TODO
    }
    else
        ; //error

    C1222_Request_Read readObj(req, tableid, NULL, 0, 0);

    return readObj;
}

/**
 * Write request
 * Detail : Transfer of table data to target
 * Header : 0x40        complete table 
 *          0x41 - 0x49 partial table using 1-9 indices
 *          0x4F        partial table using offset/octcount 
 *          
 * @param tableid   uint8_t array that contains table id (2uint8_ts)
 * @param offset    offset of table (only for partial write 0x4F)
 * @param octcount  octet count number (only for partial write 0x4F)
 * @param data      typically uint8_t array of table data 
 */

C1222_Request_Write::C1222_Request_Write():C1222_Request()
{
    request_num = 0x40;
    tableid = 0;
    index_size = 0;
    count = 0;
    data = NULL;
    index = NULL;
    build_size = 0;
}

C1222_Request_Write::~C1222_Request_Write()
{
    if(count > 0)
        delete data;
    if(index_size > 0)
        delete index;
}

C1222_Request_Write::C1222_Request_Write(uint8_t req, unsigned short tableid, 
               unsigned short * index, unsigned short index_size,
               unsigned short count, uint8_t * data)
{
    request_num = req;
    this->tableid = tableid;
    this->index_size = index_size;
    this->count = count;
    build_size = 0;

    if(this->count > 0)
    {
        this->data = new uint8_t[count];
        memcpy(this->data, data, count);
    }
    if(this->index_size > 0)
    {
        this->index = new uint8_t[index_size];
        memcpy(this->index, index, index_size);
    }
}

unsigned short 
C1222_Request_Write::get_tableid()
{
    return tableid;
}

unsigned short
C1222_Request_Write::get_count()
{
    return count;
}

uint8_t * 
C1222_Request_Write::get_data()
{
    return data;
}

uint8_t *
C1222_Request_Write::get_index()
{
    return index;
}

uint8_t *
C1222_Request_Write::build()
{
    //full write
    if(0x40 == request_num)
    {
        build_size = 1+2+2+count+1;
        raw_data = new uint8_t[build_size];
        
        raw_data[0] = request_num;
        memcpy(raw_data + 1, &tableid, 2);
        memcpy(raw_data + 3, &count, 2);
        memcpy(raw_data + 5, data, count);

        //add checksum byte
    }
    else if (0x40 < request_num && 0x49 >= request_num)
    {
        build_size = 1+2+index_size+2+count+1;
        raw_data = new uint8_t[build_size];

        raw_data[0] = request_num;
        memcpy(raw_data + 1, &tableid, 2);
        memcpy(raw_data + 3, index, index_size);
        memcpy(raw_data + 3 + index_size, &count, 2);
        memcpy(raw_data + 5 + index_size, data, count);

        //add checksum byte
    }
    else if (0x4F == request_num)
    {
        //TODO
    }
    else
        ; //error

    return raw_data;
}

C1222_Request_Write
write_parse(uint8_t * data)
{
    uint8_t req = *data;

    if(0x40 == req)
    {

    }
    else if (0x40 < req && 0x49 >= req)
    {


    }
    else if (0x4F == req)
    {

    }
    else
        ; //error

    C1222_Request_Write writeObj;

    return writeObj;
}


/**
 * Terminate request
 * Detail : abortion of the session that was established by logon req
 *          used for external/intenral error, security issues, and 
 *          any other reasons 
 * Header : 0x21
 */
C1222_Request_Terminate::C1222_Request_Terminate():C1222_Request()
{
    request_num = 0x21;
    build_size = 0;
}

uint8_t *
C1222_Request_Terminate::build()
{
    raw_data = new uint8_t[1];
    raw_data[0] = request_num;
    build_size = 1;

    return raw_data;  
}

/** 
 * Wait request
 * Detail : maintain an established session during idle period 
 * Header : 0x70
 *
 * @param interval wait period in seconds 
 */
C1222_Request_Wait::C1222_Request_Wait():C1222_Request()
{
    request_num = 0x70;
    interval = 0;
}

C1222_Request_Wait::C1222_Request_Wait(uint8_t interval)
{
    request_num = 0x70;
    this->interval = interval;
}

uint8_t 
C1222_Request_Wait::get_interval()
{
    return interval;
}

uint8_t *
C1222_Request_Wait::build()
{
    build_size = 2;
    raw_data = new uint8_t[build_size];
    raw_data[0] = request_num;
    raw_data[1] = interval;
    
    return raw_data;
}

C1222_Request_Wait
wait_parse(uint * data)
{

}
/**
 * Resolve request
 * Detail : retrieve the native address of C1222 node, this address is used
 *          to communicate directly with other C1222 node in same network
 *          This request is carried calling ap title and called ap title with
 *          in ACSE 
 * Note   : this also can be used to retrieve list of addresses relay by 
 *          not providing called ap title and calling ap title (optional)
 * Header : 0x25
 *
 * @param ap_title aptitle of target node 
 */
C1222_Request_Resolve::C1222_Request_Resolve():C1222_Request()
{
    request_num = 0x25;
    ap_title = NULL;
}

C1222_Request_Resolve::C1222_Request_Resolve(const char * ap_title)
{
    request_num = 0x25;
    this->ap_title = new char[strlen(ap_title)+1];
    strcpy(this->ap_title, ap_title);
}

C1222_Request_Resolve::~C1222_Request_Resolve()
{
    if(ap_title != NULL)
        delete ap_title;
}

char *
C1222_Request_Resolve::get_ap_title()
{
    return ap_title;
}

uint8_t *
C1222_Request_Resolve::build()
{
    build_size = 1 + strlen(ap_title);
    raw_data = new uint8_t[build_size];
    raw_data[0] = request_num;
    memcpy(raw_data + 1, ap_title, strlen(ap_title));
    
    return raw_data;
}

C1222_Request_Resolve
resolve_parse(uint8_t * data)
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
C1222_Request_Trace::C1222_Request_Trace():C1222_Request()
{
    request_num = 0x26;
}

C1222_Request_Trace::C1222_Request_Trace(const char * ap_title)
{
    request_num = 0x26;
    this->ap_title = new char[strlen(ap_title)+1];
    strcpy(this->ap_title, ap_title);
}

C1222_Request_Trace::~C1222_Request_Trace()
{
    if(ap_title != NULL)
        delete ap_title;
}

char *
C1222_Request_Trace::get_ap_title()
{
    return ap_title;
}

uint8_t *
C1222_Request_Trace::build()
{
    build_size = 1 + strlen(ap_title);
    raw_data = new uint8_t[build_size];
    raw_data[0] = request_num;
    memcpy(raw_data + 1, ap_title, strlen(ap_title));
    
    return raw_data;
}

C1222_Request_Trace 
trace_parse(uint8_t * data)
{

}

/**
 * Security request
 * Detail : provide security to change table access privilege or auth
 * Note   : this request is not security mechanism for C1222 protocol 
 * Header : 0x51
 *
 * @param passwd        password (20uint8_ts fixed)
 * @param user_id_code  user identification code
 */
C1222_Request_Security::C1222_Request_Security():C1222_Request()
{

}

C1222_Request_Security::C1222_Request_Security(const char * passwd, 
                                            unsigned short user_id)
{

}

char *
C1222_Request_Security::get_passwd()
{
    return passwd;
}

unsigned short 
C1222_Request_Security::get_user_id()
{
    return userid;
}

uint8_t * 
C1222_Request_Security::build()
{
    //int pwlen = strlen(passwd);
    //if(pwlen> 20)
    //    throw std::out_of_range("Request(security): passwd out of range");

    //char pw[20];
    //this->request_num = '\x51';
    //his->raw = new uint8_t[23];

    //add header
    //this->raw[0] = this->request_num;

    //add password
    //memcpy(pw, passwd, pwlen);
    //for(int i = 0; i < (20 - pwlen); i++)
    //    memcpy(pw+pwlen, "\x20", 1);
   
    //add userid
    //std::stringstream sstream;
    //sstream << std::hex << user_id;
    //std::string ustr = sstream.str();
    //memcpy(this->raw + 21, ustr.c_str(), 2);

}

C1222_Request_Security 
security_parse(uint8_t * data)
{

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
 * @param node_type identification of node's attribute (uint8_t)
 *                  bit 0 : relay
 *                  bit 1 : master relay
 *                  bit 2 : host
 *                  bit 3 : notification host
 *                  bit 4 : auth host
 *                  bit 5 : end device (1 is 1219 device, 0 is 1222)
 *
 * @param connection_type type of connection (uint8_t)
 *                  bit 0 : broadcast_and_multicast_supported
 *                  bit 1 : message_acceptance_window_supported
 *                  bit 2 : playback_rejection_supported (filter dup)
 *                  bit 3 : reserve (0)
 *                  bit 4 : connectionless_mode_supported
 *                  bit 5 : accept_connectionless (related 4)
 *                  bit 6 : connection_mode_support (should 1 if 4 is off)
 *                  bit 7 : accept_connection (related 6)
 *
 * @param device_class manufacturer_id in table 00 or device_class (4 uint8_ts)
 * @param ap_title      aptile of node (optional)
 * @param serial_num    unique serial number of device (optional) (table 122)
 * @param addr_len      number of uint8_t in native addr
 * @param native_addr   native address to use to foward message (optional
 *                      if lower layer provides it)
 * @param reg_period    max period in seconds desired by node to elapse 
 *                      between successful re-reg request (default 0) (word24)
 */
C1222_Request_Registration::C1222_Request_Registration():C1222_Request()
{

}

C1222_Request_Registration::C1222_Request_Registration(
         uint8_t node_type,  uint8_t conn_type,
         uint8_t * device_class, const char * ap_title, 
         uint8_t * serial_num,  uint8_t addr_len, 
         uint8_t * native_addr, uint8_t * reg_period)
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
C1222_Request_Deregistration::C1222_Request_Deregistration():C1222_Request()
{

}

C1222_Request_Deregistration::C1222_Request_Deregistration(
                                        const char * ap_title)
{

}



