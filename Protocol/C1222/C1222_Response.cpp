/* ======================================================================== *
 *                                                                          *
 *                        C1222 Response Implementation                     *  
 *                                                                          *
 * ======================================================================== */

#include "C1222_Response.h"
void *
C1222_response::build(void)
{
    return raw;
}

void 
C1222_response::clear(void)
{
    delete raw;
}

/**
 * Identify response
 * 
 * @param res response isss or bsy or err or ok with parameters
 * @param std represent standard type of device (byte)
 *              0x00 : ANSI C12.18
 *              0x01 : Reserve
 *              0x02 : ANSI C12.21
 *              0x03 : ANSI C12.22
 *              0x04 - 0xFF : Reserve
 * @param ver binary representation of standard version left of decimal pts 
 * @param rev binary representation of standard version right of decimal pts
 */
void
C1222_response::identify(const byte res, const byte std, const byte ver,
        const byte rev)
{

}

/**
 * TBD
 *
 */
void 
C1222_response::read(const byte res, const byte * count, const byte * data,
        const byte chksum)
{

}

/**
 * TBD
 *
 */
void
C1222_response::write(const byte res)
{

}

/**
 * Logon response
 *
 * @param res       response state 
 * @param timeout   seconds to be idle before termination (word16)
 */
void
C1222_response::logon(const byte res, const byte * timeout)
{

}

/**
 * Security response
 *
 * @param res       response state
 */
void
C1222_response::security(const byte res)
{

}

/**
 * Logoff response
 *
 * @param res       response state
 */
void
C1222_response::logoff(const byte res)
{

}

/**
 * Terminate response
 *
 * @param res       response state
 */
void
C1222_response::terminate(const byte res)
{

}

/**
 * Disconnect response
 *
 * @param res       response state
 */
void
C1222_response::disconnect(const byte res)
{

}

/**
 * Wait response
 *
 * @param res       response state
 */
void
C1222_response::wait(const byte res)
{

}

/**
 * Registration response
 *
 * @param res       response state
 * @param ap_title  registred ap title
 * @param delay     maximum delay in seconds that device should waits
 * @param period    maximum period in seconds allowed to elapse re-reg
 * @param info      Node info about connection type (see Reg. request)
 */
void
C1222_response::registration(const byte res, const byte * ap_title,
        const byte * delay, const byte * period, const byte info)
{

}

/**
 * Deregistration response
 *
 * @param res       response state
 */
void
C1222_response::deregistration(const byte res)
{

}

/**
 * Resolve response
 *
 * @param res       response state
 * @param addr_len  address length 
 * @param addr      local address of requested aptitle
 */
void
C1222_response::resolve(const byte res, const byte addr_len, const byte * addr)
{

}

/**
 * Resolve response
 *
 * @param res       response state
 * @param ap_titles aptitle of c12.22 relays used to forward this request
 */
void
trace(const byte res, const byte ** aptitles)
{

}
