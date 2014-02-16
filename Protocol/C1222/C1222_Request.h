/* ========================================================================= *
 *                                                                           *
 *                            C1222 Request class                            *  
 *                                                                           *
 * ========================================================================= */

#ifndef __C1222_REQUEST_H__
#define __C1222_REQUEST_H__

class C1222_Request : public C1222 {
    public:
        void * build();
        void clear();

        void disconnect(void);
        
        void identify(void);
        
        void logoff(void);
        
        void logon(const byte * user_id_code, const byte * id, 
                const byte * timeout);

        void write(const byte * tableid, const byte * offset,
                const byte * octcount, void * data);

        void read(const byte * tableid, const byte * offset,
                const byte * octcount);

        void wait(const byte interval);
        
        void terminate(void);
        
        void security(const byte * passwd, const byte * user_id_code);
        
        void resolve(const byte * ap_title);
        
        void registration(const byte node_type, const byte conn_type,
                const byte * device_class, const byte * ap_title,
                const byte * serial_num, const byte addr_len, 
                const byte * native_addr, const byte * reg_period);

        void deregistration(const byte * ap_title);
        
        void trace(const byte * ap_title);

    private:
        byte request_num;
        byte * raw;
};

#endif
