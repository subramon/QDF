#include "incs.h"
#include "qdf_struct.h"
#include "vsrem_I1.h" 
#include "vsrem_I2.h" 
#include "vsrem_I4.h" 
#include "vsrem_I8.h" 
#include "vsrem_UI1.h" 
#include "vsrem_UI2.h" 
#include "vsrem_UI4.h" 
#include "vsrem_UI8.h" 
/*
#include "vsrem_F4.h" 
#include "vsrem_F8.h" 
*/
extern int
vsrem(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    );
    
