/* ======================================================================== *
 *                                                                          *
 *                        C1222 Response Implementation                     *  
 *                                                                          *
 * ======================================================================== */

#include "C1222_Response.h"
#include <stdlib.h>
#include <string.h>

    
void *
C1222_Response::build(void)
{
    return raw;
}

void 
C1222_Response::clear(void)
{
    delete raw;
}

long
C1222_Response::get_data_len()
{
    return this->datalen;
}
/**
 * Identify Response
 * 
 * @param res Response isss or bsy or err or ok with parameters
 * @param std represent standard type of device (uint8_t)
 *              0x00 : ANSI C12.18
 *              0x01 : Reserve
 *              0x02 : ANSI C12.21
 *              0x03 : ANSI C12.22
 *              0x04 - 0xFF : Reserve
 * @param ver binary representation of standard version left of decimal pts 
 * @param rev binary representation of standard version right of decimal pts
 */
void
C1222_Response::identify(const uint8_t res, const uint8_t std, const uint8_t ver,
        const uint8_t rev)
{
    if(res != RES_OK)
    {
        this->raw = new uint8_t[1];
        this->raw[0] = res;
    }
    else
    {
        this->raw = new uint8_t[4];
        this->raw[0] = RES_OK;
        this->raw[1] = std;
        this->raw[2] = ver;
        this->raw[3] = rev;
    }
}

/**
 * TBD
 *
 */
void 
C1222_Response::read(const uint8_t res, const uint8_t * count, const uint8_t * data,
        const uint8_t chksum)
{

}

/**
 * TBD
 *
 */
void
C1222_Response::write(const uint8_t res)
{

}

/**
 * Logon Response
 *
 * @param res       Response state 
 * @param timeout   seconds to be idle before termination (word16)
 */
void
C1222_Response::logon(const uint8_t res, const uint8_t * timeout)
{
    if(res != RES_OK)
    {
        this->raw = new uint8_t[1];
        this->raw[0] = res;
    }
    else
    {
        this->raw = new uint8_t[3];
        this->raw[0] = RES_OK;   
        memcpy(this->raw + 1, timeout, 2);
    }
}

/**
 * Security Response
 *
 * @param res       Response state
 */
void
C1222_Response::security(const uint8_t res)
{
    this->raw = new uint8_t[1];
    this->raw[0] = res;
}

/**
 * Logoff Response
 *
 * @param res       Response state
 */
void
C1222_Response::logoff(const uint8_t res)
{
    this->raw = new uint8_t[1];
    this->raw[0] = res;
}

/**
 * Terminate Response
 *
 * @param res       Response state
 */
void
C1222_Response::terminate(const uint8_t res)
{
    this->raw = new uint8_t[1];
    this->raw[0] = res;
}

/**
 * Disconnect Response
 *
 * @param res       Response state
 */
void
C1222_Response::disconnect(const uint8_t res)
{
    this->raw = new uint8_t[1];
    this->raw[0] = res;
}

/**
 * Wait Response
 *
 * @param res       Response state
 */
void
C1222_Response::wait(const uint8_t res)
{
    this->raw = new uint8_t[1];
    this->raw[0] = res;
}

/**
 * Registration Response
 *
 * @param res       Response state
 * @param ap_title  registred ap title
 * @param delay     maximum delay in seconds that device should waits
 * @param period    maximum period in seconds allowed to elapse re-reg
 * @param info      Node info about connection type (see Reg. request)
 */
void
C1222_Response::registration(const uint8_t res, const uint8_t * ap_title,
        const uint8_t * delay, const uint8_t * period, const uint8_t info)
{

}

/**
 * Deregistration Response
 *
 * @param res       Response state
 */
void
C1222_Response::deregistration(const uint8_t res)
{
    this->raw = new uint8_t[1];
    this->raw[0] = res;
}

/**
 * Resolve Response
 *
 * @param res       Response state
 * @param addr_len  address length 
 * @param addr      local address of requested aptitle
 */
void
C1222_Response::resolve(const uint8_t res, const uint8_t addr_len, 
        const uint8_t * addr)
{

}

/**
 * trace  Response
 *
 * @param res       Response state
 * @param ap_titles aptitle of c12.22 relays used to forward this request
 */
void
C1222_Response::trace(const uint8_t res, const uint8_t ** aptitles)
{

}

void
C1222_Response::parse_response(void * data)
{


}
