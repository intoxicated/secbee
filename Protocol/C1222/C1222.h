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
} ap_element;

class C1222 {
    virtual void * build() = 0;
};

#endif






