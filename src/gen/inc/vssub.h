#include "incs.h"
#include "qdf_struct.h"
#ifndef ISPC
#include "vssub_I1.h" 
#include "vssub_I2.h" 
#include "vssub_I4.h" 
#include "vssub_I8.h" 
#include "vssub_F4.h" 
#include "vssub_F8.h" 
#else
#include "ispc_vssub_I1.h" 
#include "ispc_vssub_I2.h" 
#include "ispc_vssub_I4.h" 
#include "ispc_vssub_I8.h" 
#include "ispc_vssub_F4.h" 
#include "ispc_vssub_F8.h" 
#endif
extern int
vssub(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    );
    
