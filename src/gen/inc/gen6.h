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
    
