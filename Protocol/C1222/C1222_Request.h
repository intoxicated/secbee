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
        C1222_Request();
        ~C1222_Request();

        uint8_t *     build();
        uint8_t       get_request_num();
        unsigned long get_build_size();

    protected:
        uint8_t         request_num;
        uint8_t *       raw_data;
        unsigned long   build_size;
};



class C1222_Request_Ident : public C1222_Request
{
    public: 
        C1222_Request_Ident();
        uint8_t * build();
};

class C1222_Request_Logoff : public C1222_Request
{
    public: 
        C1222_Request_Logoff();
        uint8_t * build();
};

class C1222_Request_Disconnect : public C1222_Request
{
    public: 
        C1222_Request_Disconnect();
        uint8_t * build();
};

class C1222_Request_Logon : public C1222_Request
{
    public: 
        C1222_Request_Logon();
        C1222_Request_Logon(unsigned short user_id, 
                        const  char *  username, 
                        unsigned short timeout);

        uint8_t *      build();

        unsigned short get_user_id();
        unsigned short get_timeout();
        char *         get_username();

        static C1222_Request_Logon * parse(uint8_t * data);

    private:
        unsigned short user_id;
        unsigned short timeout;
        char username[11];
};

class C1222_Request_Write : public C1222_Request
{
    public: 
        C1222_Request_Write();
        ~C1222_Request_Write();
        C1222_Request_Write(uint8_t req, unsigned short tabldid, 
               unsigned short * index, unsigned short index_size,
               unsigned short count, uint8_t * data);

        uint8_t *           build();
        uint8_t *           get_data();
        uint8_t *           get_index();
        unsigned short      get_tableid();
        unsigned short      get_count();

        static C1222_Request_Write * parse(uint8_t * data);

    private:
        unsigned short      tableid;
        unsigned short      index_size;
        unsigned short      count;
        uint8_t *           index;
        uint8_t *           data;

};

class C1222_Request_Read : public C1222_Request
{
    public:
        C1222_Request_Read();
        ~C1222_Request_Read();
        C1222_Request_Read(uint8_t req, unsigned short tableid,
                uint8_t * index, unsigned short index_size, 
                unsigned short e_count);

        uint8_t *         build();
        uint8_t *         get_index();
        unsigned short    get_tableid();
        unsigned short    get_e_count();

        static C1222_Request_Read * parse(uint8_t * data);

    private:
        unsigned short      tableid;
        uint8_t *           index;
        unsigned short      index_size;
        unsigned short      e_count;
};

class C1222_Request_Terminate : public C1222_Request
{
    public:
        C1222_Request_Terminate();
        uint8_t * build();
};

class C1222_Request_Wait : public C1222_Request
{
    public:
        C1222_Request_Wait();
        C1222_Request_Wait(uint8_t interval);

        uint8_t *      build();
        uint8_t        get_interval();

        static C1222_Request_Wait * parse(uint8_t * data);

    private:
        uint8_t interval;
};

class C1222_Request_Resolve : public C1222_Request
{
    public:
        C1222_Request_Resolve();
        ~C1222_Request_Resolve();
        C1222_Request_Resolve(const char * ap_title);

        uint8_t *      build();
        char *         get_ap_title();

        static C1222_Request_Resolve * parse(uint8_t * data);

    private:
        char * ap_title;
};

class C1222_Request_Trace : public C1222_Request
{
    public:
        C1222_Request_Trace();
        ~C1222_Request_Trace();
        C1222_Request_Trace(const char * ap_title);

        uint8_t *       build();
        char *          get_ap_title();

        static C1222_Request_Trace * parse(uint8_t * data);

    private:
        char * ap_title;
};

class C1222_Request_Security : public C1222_Request
{
    public:
        C1222_Request_Security();
        C1222_Request_Security(const char * passwd, unsigned short user_id);

        uint8_t *       build();
        char *          get_passwd();
        unsigned short  get_user_id();

        static C1222_Request_Security * parse(uint8_t * data);

    private:
        char *         passwd;
        unsigned short userid;
};

class C1222_Request_Registration : public C1222_Request
{
    public:
        C1222_Request_Registration();
        C1222_Request_Registration( uint8_t node_type, uint8_t conn_type,
            uint8_t * device_class, const char * ap_title, 
            uint8_t * serial_num,  uint8_t addr_len, 
            uint8_t * native_addr, uint8_t * reg_period);

        static C1222_Request_Registration * parse(uint8_t * data);

    private:
        uint8_t     node_type;
        uint8_t     conn_type;
        uint8_t     addr_len;
        uint8_t *   device_class;
        uint8_t *   serial_num;
        uint8_t *   native_addr;
        uint8_t *   reg_period;
        char *      ap_title;
};

class C1222_Request_Deregistration : public C1222_Request
{
    public:
        C1222_Request_Deregistration();
        C1222_Request_Deregistration(const char * ap_title);

        char *  get_ap_title();

        static C1222_Request_Deregistration * parse(uint8_t * data);

    private:
        char * ap_title;
};

class C1222_Request_Parser {
    public:
        uint8_t req;

        static void * parse(uint8_t * data)
        {
            return C1222_Request_Read::parse(data);
        }
};

#endif
