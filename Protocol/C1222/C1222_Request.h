/* ========================================================================= *
 *                                                                           *
 *                            C1222 Request class                            *  
 *                                                                           *
 * ========================================================================= */

#ifndef __C1222_REQUEST_H__
#define __C1222_REQUEST_H__

#include "C1222.h"
#include <string>

class C1222_Request : public C1222 {
    public:
        void * build();
        void clear();

        void disconnect(void);
        
        void identify(void);
        
        void logoff(void);
        
        void logon(const unsigned short user_id, const std::string username, 
                const unsigned short timeout);

        void write(const byte * tableid, const byte * offset,
                const byte * octcount, void * data);

        void read(const byte * tableid, const byte * offset,
                const byte * octcount);

        void wait(const byte interval);
        
        void terminate(void);
        
        void security(const std::string passwd, const unsigned short user_id);
        
        void resolve(const byte * ap_title);
        
        void registration(const byte node_type, const byte conn_type,
                const byte * device_class, const byte * ap_title,
                const byte * serial_num, const byte addr_len, 
                const byte * native_addr, const byte * reg_period);

        void deregistration(const byte * ap_title);
        
        void trace(const std::string ap_title);

    private:
        byte request_num;
        byte * raw;
};

#endif
