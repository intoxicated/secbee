/* ===================================================================== *
 *                                                                       *
 *                                                                       *
 *                                                                       *
 *                                                                       *
 * ===================================================================== */


#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>

#include <xbee.h>

#include "../Protocol/C1222/C1222.h"
#include "../Protocol/C1222/C1222_ACSE.h"
#include "../Protocol/C1222/C1222_EPSEM.h"
#include "../Protocol/C1222/C1222_Request.h"

#include "Secbee.h"

/* Example of logon request
 * after stacking all necessary part it should be looked like this
 *
 * 60 29                                            <acse-pdu>
 *     A2 05                                        <called ap title element>
 *          80 03 7B 75                             <called ap title> = .123.8437
 *     A6 04                                        <calling ap title element>
 *          80 02 7B                                <calling ap title> = .123.4
 *     AB 03                                        <calling ap invo id element> 
 *          02 01 07                                <calling ap invo id> = 7
 *     BE 15                                        <user information element>
 *          28 13                                   <user info external>
 *              81 11                               <user info octet string>
 *                  80                              <epsem control>
 *                  0F                              <service len>
 *                  50                              <logon>
 *                  00 02                           <user id> = 2
 *                  55 53 45 52 20 4E 41 4D 45 20   <user> = "USER NAME"
 *                  00 3C                           <timeout> = 60sec
 */

int main(int argc, char ** argv)
{
    C1222_Request req;

}
