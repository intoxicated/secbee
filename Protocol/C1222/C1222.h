/* ======================================================================== *
 *                                                                          *
 *                            C1222 base class                              *  
 *                                                                          *
 * ======================================================================== */


#ifndef __C1222_BASE_H__
#define __C1222_BASE_H__

//typedef unsigned char uint8_t;
#include <stdint.h>

typedef struct {
    uint8_t * data;
    int size;
} element;

class C1222 {
    virtual uint8_t * build() = 0;
};

#endif






