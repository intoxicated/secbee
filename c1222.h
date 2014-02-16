/**
 * C1222 packet construction
 *
 * Wooyoung Chung
 */

typedef unsigned char byte;

/**
 * EPSEM format representation class
 */
class c1222 {
    virtual void * build() = 0;
};

/**
 * C1222 packet representation 
 */
class c1222_packet : public c1222 {
    public:
        c1222_packet(char * called_ap, char * called_id, 
            char * calling_ap, char * calling_id, void * data, int len);
        void * build();
        friend c1222_packet& parse(void * data);
    
    private:
        void * data;
        char * called_ap, * called_ap_invo;
        char * calling_ap, * calling_ap_invo;
        int length;
};

/**
 * epsem format representation
 *
 */
class c1222_epsem : public c1222{
    public:
        c1222_epsem(void * data, int e_class);

        int getType();
        bool getFlags(int flag);
        void * build();

        friend c1222_epsem& epsem_parse(void * data);

    private:
        void * e_data; 
        int header, e_class;
        bool reserved, recovery, proxy, security, response;
};

/**
 * ACSE format representation class
 */
class c1222_acse : public c1222 {
    public:
        void * build();
        friend c1222_acse& acse_parse(void * data);

    private:
        char * calling_ap_invo;
};

/**
 * C1222 request representation class
 */
class c1222_request : public c1222 {
    public:
        void * build();
        void clear();

        //request methods
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

class c1222_response : public c1222 {
    void * build();
    void clear();

    //response methods
};
