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

class C1222_Request : public C1222 {
    public:
        void * build();
        void clear();

        void disconnect(void);
        
        void identify(void);
        
        void logoff(void);
        
        void logon(const unsigned short user_id, const std::string username, 
                const unsigned short timeout);

        void write(const unsigned short tableid, const uint8_t * offset,
                const uint8_t * octcount, void * data);

        void read(const unsigned short tableid, const uint8_t * offset,
                const uint8_t * octcount);

        void wait(const uint8_t interval);
        
        void terminate(void);
        
        void security(const std::string passwd, const unsigned short user_id);
        
        void resolve(const std::string ap_title);
        
        void registration(const uint8_t node_type, const uint8_t conn_type,
                const uint8_t * device_class, const std::string ap_title,
                const uint8_t * serial_num, const uint8_t addr_len, 
                const uint8_t * native_addr, const uint8_t * reg_period);

        void deregistration(const std::string ap_title);
        
        void trace(const std::string ap_title);

    private:
        uint8_t request_num;
        uint8_t * raw;
};

#endif
