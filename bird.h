#ifndef BIRD_H
#define BIRD_H

#include "C12832.h"

static char bird[] = { 
0x03, 0xF0,    //____ __XX XXXX ____ ____
0x0C, 0x28,    //____ XX__ __X_ X___ ____
0x10, 0x44,    //___X ____ _X__ _X__ ____
0x78, 0x4A,    //_XXX X___ _X__ X_X_ ____
0x84, 0x4A,    //X___ _X__ _X__ X_X_ ____
0x82, 0x22,    //X___ __X_ __X_ __X_ ____
0x82, 0x1F,    //X___ __X_ ___X XXXX ____
0x44, 0x21,    //_X__ _X__ __X_ ____ X___
0x38, 0x5F,    //__XX X___ _X_X XXXX ____
0x08, 0x21,    //____ X___ __X_ ___X ____
0x06, 0x1F,    //____ _XX_ ___X XXXX ____
0x01, 0xE0    //____ ___X XXX_ ____ ____
    };
    
Bitmap bitmapbird = {
    16,
    12,
    2,
    bird
};

#endif