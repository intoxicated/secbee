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
        char * called_ap, * called_ap_invo; //destination 
        char * calling_ap, * calling_ap_invo; //source
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


/**
 * response class (purpose to work on prototype) 
 *
 */

//minimal response code
#define RES_OK      '\x00' //ok 
#define RES_ERR     '\x01' //error 
#define RES_SNS     '\x02' //service not support
#define RES_ONP     '\x04' //operation not possible 
#define RES_BSY     '\x06' //device busy
#define RES_ISSS    '\x0A' //invalid serviece sequence state
#define RES_SME     '\x0B' //security mechanism error 
#define RES_UAT     '\x0C' //unknown application title (aptitle)
#define RES_SGNP    '\x11' //segment not possible


class c1222_response : public c1222 {
    public:
        void * build(void);
        void clear(void);
    
        //feature excluded but necessary to add..
        void identify(const byte res, const byte std, const byte ver, 
                const byte rev);  

        void read(const byte res, const byte * count, const byte * data,
                const byte chksum);

        void write(const byte res);

        void logon(const byte res, const byte * timeout);

        void security(const byte res);

        void logoff(const byte res);

        void terminate(const byte res);

        void disconnect(const byte res);

        void wait(const byte res);

        void registration(const byte res, const byte * ap_title, 
                const byte * delay, const byte * period, const byte info);

        void deregistration(const byte res);

        void resolve(const byte res, const byte addr_len, const byte * addr);

        void trace(const byte res, const byte ** aptitles);

    private:
        byte * raw; 
};
