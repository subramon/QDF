#include "vvadd.h" 
#include "vvadd_I1.h" 
#include "vvadd_I2.h" 
#include "vvadd_I4.h" 
#include "vvadd_I8.h" 
#include "vvadd_F4.h" 
#include "vvadd_F8.h" 
#include "ispc_vvadd_I1.h" 
#include "ispc_vvadd_I2.h" 
#include "ispc_vvadd_I4.h" 
#include "ispc_vvadd_I8.h" 
#include "ispc_vvadd_F4.h" 
#include "ispc_vvadd_F8.h" 
int
vvadd(
    void *x,
    void * y,
    uint32_t n,
    qtype_t xqtype,
    void *z
    )
{
  int status = 0; 

  switch ( xqtype ) { 
    //-----------------------------------------
    case I1 :
#ifndef ISPC
      vvadd_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#else
      ispc_vvadd_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#endif
      break;
    //-----------------------------------------
    case I2 :
#ifndef ISPC
      vvadd_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#else
      ispc_vvadd_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#endif
      break;
    //-----------------------------------------
    case I4 :
#ifndef ISPC
      vvadd_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#else
      ispc_vvadd_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#endif
      break;
    //-----------------------------------------
    case I8 :
#ifndef ISPC
      vvadd_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#else
      ispc_vvadd_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#endif
      break;
    //-----------------------------------------
    case F4 :
#ifndef ISPC
      vvadd_F4((float    *)x, (float    *)y, n, (float    *)z);
#else
      ispc_vvadd_F4((float    *)x, (float    *)y, n, (float    *)z);
#endif
      break;
    //-----------------------------------------
    case F8 :
#ifndef ISPC
      vvadd_F8((double *)x, (double *)y, n, (double *)z);
#else
      ispc_vvadd_F8((double *)x, (double *)y, n, (double *)z);
#endif
      break;
    //-----------------------------------------
    default : 
      go_BYE(-1); 
      break;
  }
BYE:
  return status;
}
#include "vvsub.h" 
#include "vvsub_I1.h" 
#include "vvsub_I2.h" 
#include "vvsub_I4.h" 
#include "vvsub_I8.h" 
#include "vvsub_F4.h" 
#include "vvsub_F8.h" 
#include "ispc_vvsub_I1.h" 
#include "ispc_vvsub_I2.h" 
#include "ispc_vvsub_I4.h" 
#include "ispc_vvsub_I8.h" 
#include "ispc_vvsub_F4.h" 
#include "ispc_vvsub_F8.h" 
int
vvsub(
    void *x,
    void * y,
    uint32_t n,
    qtype_t xqtype,
    void *z
    )
{
  int status = 0; 

  switch ( xqtype ) { 
    //-----------------------------------------
    case I1 :
#ifndef ISPC
      vvsub_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#else
      ispc_vvsub_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#endif
      break;
    //-----------------------------------------
    case I2 :
#ifndef ISPC
      vvsub_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#else
      ispc_vvsub_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#endif
      break;
    //-----------------------------------------
    case I4 :
#ifndef ISPC
      vvsub_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#else
      ispc_vvsub_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#endif
      break;
    //-----------------------------------------
    case I8 :
#ifndef ISPC
      vvsub_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#else
      ispc_vvsub_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#endif
      break;
    //-----------------------------------------
    case F4 :
#ifndef ISPC
      vvsub_F4((float    *)x, (float    *)y, n, (float    *)z);
#else
      ispc_vvsub_F4((float    *)x, (float    *)y, n, (float    *)z);
#endif
      break;
    //-----------------------------------------
    case F8 :
#ifndef ISPC
      vvsub_F8((double *)x, (double *)y, n, (double *)z);
#else
      ispc_vvsub_F8((double *)x, (double *)y, n, (double *)z);
#endif
      break;
    //-----------------------------------------
    default : 
      go_BYE(-1); 
      break;
  }
BYE:
  return status;
}
#include "vvmul.h" 
#include "vvmul_I1.h" 
#include "vvmul_I2.h" 
#include "vvmul_I4.h" 
#include "vvmul_I8.h" 
#include "vvmul_F4.h" 
#include "vvmul_F8.h" 
#include "ispc_vvmul_I1.h" 
#include "ispc_vvmul_I2.h" 
#include "ispc_vvmul_I4.h" 
#include "ispc_vvmul_I8.h" 
#include "ispc_vvmul_F4.h" 
#include "ispc_vvmul_F8.h" 
int
vvmul(
    void *x,
    void * y,
    uint32_t n,
    qtype_t xqtype,
    void *z
    )
{
  int status = 0; 

  switch ( xqtype ) { 
    //-----------------------------------------
    case I1 :
#ifndef ISPC
      vvmul_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#else
      ispc_vvmul_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#endif
      break;
    //-----------------------------------------
    case I2 :
#ifndef ISPC
      vvmul_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#else
      ispc_vvmul_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#endif
      break;
    //-----------------------------------------
    case I4 :
#ifndef ISPC
      vvmul_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#else
      ispc_vvmul_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#endif
      break;
    //-----------------------------------------
    case I8 :
#ifndef ISPC
      vvmul_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#else
      ispc_vvmul_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#endif
      break;
    //-----------------------------------------
    case F4 :
#ifndef ISPC
      vvmul_F4((float    *)x, (float    *)y, n, (float    *)z);
#else
      ispc_vvmul_F4((float    *)x, (float    *)y, n, (float    *)z);
#endif
      break;
    //-----------------------------------------
    case F8 :
#ifndef ISPC
      vvmul_F8((double *)x, (double *)y, n, (double *)z);
#else
      ispc_vvmul_F8((double *)x, (double *)y, n, (double *)z);
#endif
      break;
    //-----------------------------------------
    default : 
      go_BYE(-1); 
      break;
  }
BYE:
  return status;
}
#include "vvdiv.h" 
#include "vvdiv_I1.h" 
#include "vvdiv_I2.h" 
#include "vvdiv_I4.h" 
#include "vvdiv_I8.h" 
#include "vvdiv_F4.h" 
#include "vvdiv_F8.h" 
// #include "vvdiv_I1.h" 
// #include "vvdiv_I2.h" 
// #include "vvdiv_I4.h" 
// #include "vvdiv_I8.h" 
#include "ispc_vvdiv_F4.h" 
#include "ispc_vvdiv_F8.h" 
int
vvdiv(
    void *x,
    void * y,
    uint32_t n,
    qtype_t xqtype,
    void *z
    )
{
  int status = 0; 

  switch ( xqtype ) { 
    //-----------------------------------------
    case I1 :
#ifndef ISPC
      vvdiv_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#else
      vvdiv_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#endif
      break;
    //-----------------------------------------
    case I2 :
#ifndef ISPC
      vvdiv_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#else
      vvdiv_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#endif
      break;
    //-----------------------------------------
    case I4 :
#ifndef ISPC
      vvdiv_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#else
      vvdiv_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#endif
      break;
    //-----------------------------------------
    case I8 :
#ifndef ISPC
      vvdiv_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#else
      vvdiv_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#endif
      break;
    //-----------------------------------------
    case F4 :
#ifndef ISPC
      vvdiv_F4((float    *)x, (float    *)y, n, (float    *)z);
#else
      ispc_vvdiv_F4((float    *)x, (float    *)y, n, (float    *)z);
#endif
      break;
    //-----------------------------------------
    case F8 :
#ifndef ISPC
      vvdiv_F8((double *)x, (double *)y, n, (double *)z);
#else
      ispc_vvdiv_F8((double *)x, (double *)y, n, (double *)z);
#endif
      break;
    //-----------------------------------------
    default : 
      go_BYE(-1); 
      break;
  }
BYE:
  return status;
}
