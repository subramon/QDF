#include "incs.h"
#include "qdf_struct.h"
#ifndef ISPC
#include "vsmul_I1.h" 
#include "vsmul_I2.h" 
#include "vsmul_I4.h" 
#include "vsmul_I8.h" 
#include "vsmul_F4.h" 
#include "vsmul_F8.h" 
#else
#include "ispc_vsmul_I1.h" 
#include "ispc_vsmul_I2.h" 
#include "ispc_vsmul_I4.h" 
#include "ispc_vsmul_I8.h" 
#include "ispc_vsmul_F4.h" 
#include "ispc_vsmul_F8.h" 
#endif
extern int
vsmul(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    );
    
