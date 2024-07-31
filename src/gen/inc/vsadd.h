#include "incs.h"
#include "qdf_struct.h"
#ifndef ISPC
#include "vsadd_I1.h" 
#include "vsadd_I2.h" 
#include "vsadd_I4.h" 
#include "vsadd_I8.h" 
#include "vsadd_F4.h" 
#include "vsadd_F8.h" 
#else
#include "ispc_vsadd_I1.h" 
#include "ispc_vsadd_I2.h" 
#include "ispc_vsadd_I4.h" 
#include "ispc_vsadd_I8.h" 
#include "ispc_vsadd_F4.h" 
#include "ispc_vsadd_F8.h" 
#endif
extern int
vsadd(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    );
    
