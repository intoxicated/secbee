/* ========================================================================= *
 *                                                                           *
 *                            C1222 Request class                            *  
 *                                                                           *
 * ========================================================================= */

#ifndef __C1222_REQUEST_H__
#define __C1222_REQUEST_H__

#include <string>
#include <stdint.h>

#include "C1222.h"

using namespace std;

class C1222_Request : public C1222 {
    public:
        void * build();
        void clear();

        void disconnect(void);
        
        void identify(void);
        
        void logoff(void);
        
        void logon(const unsigned short user_id, const char * username, 
                const unsigned short timeout);

        void write(const unsigned short tableid, const uint8_t * offset,
                const uint8_t * octcount, void * data);

        void read(const unsigned short tableid, const uint8_t * offset,
                const uint8_t * octcount);

        void wait(const uint8_t interval);
        
        void terminate(void);
        
        void security(const char * passwd, const unsigned short user_id);
        
        void resolve(const char * ap_title);
        
        void registration(const uint8_t node_type, const uint8_t conn_type,
                const uint8_t * device_class, const char * ap_title,
                const uint8_t * serial_num, const uint8_t addr_len, 
                const uint8_t * native_addr, const uint8_t * reg_period);

        void deregistration(const char * ap_title);
        
        void trace(const char * ap_title);

        static void parse_request(void * data);
    private:
        uint8_t request_num;
        uint8_t * raw;
};

#endif

/*
class C1222_Request_Ident : public C1222_Request
{

};

class C1222_Request_Logoff : public C1222_Request
{

};

class C1222_Request_Logon : public C1222_Request
{

};

class C1222_Request_Write : public C1222_Request
{

};

class C1222_Request_Read : public C1222_Request
{

};

class C1222_Request_Terminate : public C1222_Request
{

};

class C1222_Request_Wait : public C1222_Request
{

};

class C1222_Request_Resolve : public C1222_Request
{

};

class C1222_Request_Trace : public C1222_Request
{

};
*/

