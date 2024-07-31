#include "incs.h"
#include "qdf_struct.h"
#ifndef ISPC
#include "vsdiv_I1.h" 
#include "vsdiv_I2.h" 
#include "vsdiv_I4.h" 
#include "vsdiv_I8.h" 
#include "vsdiv_F4.h" 
#include "vsdiv_F8.h" 
#else
#include "vsdiv_I1.h" 
#include "vsdiv_I2.h" 
#include "vsdiv_I4.h" 
#include "vsdiv_I8.h" 
#include "vsdiv_F4.h" 
#include "vsdiv_F8.h" 
#endif
extern int
vsdiv(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    );
    
