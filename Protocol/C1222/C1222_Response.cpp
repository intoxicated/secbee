/* ======================================================================== *
 *                                                                          *
 *                        C1222 Response Implementation                     *  
 *                                                                          *
 * ======================================================================== */

#include "C1222_Response.h"
#include <stdlib.h>
#include <string.h>


uint8_t *
C1222_Response::build(void)
{
    return raw_data;
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
C1222_Response::identify(uint8_t res, uint8_t std, uint8_t ver, uint8_t rev)
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
C1222_Response::read(uint8_t res, uint8_t * count, uint8_t * data,
        const uint8_t chksum)
{

}

/**
 * TBD
 *
 */
void
C1222_Response::write(uint8_t res)
{

}

/**
 * Logon Response
 *
 * @param res       Response state 
 * @param timeout   seconds to be idle before termination (word16)
 */
void
C1222_Response::logon(uint8_t res, uint8_t * timeout)
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
C1222_Response::security(uint8_t res)
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
C1222_Response::logoff(uint8_t res)
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
C1222_Response::terminate(uint8_t res)
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
C1222_Response::disconnect(uint8_t res)
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
C1222_Response::wait(uint8_t res)
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
C1222_Response::registration(uint8_t res, const uint8_t * ap_title,
         uint8_t * delay,  uint8_t * period,  uint8_t info)
{

}

/**
 * Deregistration Response
 *
 * @param res       Response state
 */
void
C1222_Response::deregistration(uint8_t res)
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
C1222_Response::resolve(uint8_t res, uint8_t addr_len, uint8_t * addr)
{

}

/**
 * trace  Response
 *
 * @param res       Response state
 * @param ap_titles aptitle of c12.22 relays used to forward this request
 */
void
C1222_Response::trace( uint8_t res, uint8_t ** aptitles)
{

}

/**
 * parse response data and convert to human readable format
 * 
 * @param data response data
 * @param req request has been submitted
 */
void
C1222_Response::parse_response(void * data, uint8_t req)
{


}
