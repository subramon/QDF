#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "flatten_sawtooth.h"
// Following is candidate for code generation
int
flatten_sawtooth(
    const void * const src,
    uint32_t n,
    qtype_t qtype,
    void *dst
    )
{
  int status = 0;
  switch ( qtype ) { 
    case I1 : flatten_sawtooth_I1(src, n, dst); cBYE(status); break;
    case I2 : flatten_sawtooth_I2(src, n, dst); cBYE(status); break;
    case I4 : flatten_sawtooth_I4(src, n, dst); cBYE(status); break;
    case I8 : flatten_sawtooth_I8(src, n, dst); cBYE(status); break;
//    case Q0 : case BL : 
//    case F2 : case F4 : case F8 : 
//    case UI1 : case UI2 : case UI4 : case UI8 : 
//    case SC: case SV : case TM : case HL :  
    default : go_BYE(-1); 
  }
BYE:
  return status;
}
// Following is candidate for code generation
// TODO P4 Allow differnet types of src and dst 
int
flatten_sawtooth_I1(
    const void * const src,
    uint32_t n,
    void *dst
    )
{
  int status = 0;
  const int8_t * const s = (const int8_t *const)src;
  int8_t *d = (int8_t *)dst;

  if ( src == NULL ) { go_BYE(-1); } 
  if ( dst == NULL ) { go_BYE(-1); } 
  if ( n == 0 ) { go_BYE(-1); }
  int8_t s_right = s[n-1];
  for ( int i = (int)n-1; i >= 0; i-- ) { 
#ifdef DEBUG
    if ( s[i] < 0 ) { go_BYE(-1); } 
#endif
    if ( s[i] == 0 ) { d[i] = 0; s_right = 0; continue; } 
    //-------------------------------
    if ( s_right == 0 ) { s_right = d[i] = s[i]; continue; } 
    d[i] = s_right;
  }
BYE:
  return status;
}
int
flatten_sawtooth_I2(
    const void * const src,
    uint32_t n,
    void *dst
    )
{
  int status = 0;
  const int16_t * const s = (const int16_t * const)src;
  int16_t *d = (int16_t *)dst;

  if ( src == NULL ) { go_BYE(-1); } 
  if ( dst == NULL ) { go_BYE(-1); } 
  if ( n == 0 ) { go_BYE(-1); }
  int16_t s_right = s[n-1];
  for ( int i = (int)n-1; i >= 0; i-- ) { 
#ifdef DEBUG
    if ( s[i] < 0 ) { go_BYE(-1); } 
#endif
    if ( s[i] == 0 ) { d[i] = 0; s_right = 0; continue; } 
    //-------------------------------
    if ( s_right == 0 ) { s_right = d[i] = s[i]; continue; } 
    d[i] = s_right;
  }
BYE:
  return status;
}
int
flatten_sawtooth_I4(
    const void * const src,
    uint32_t n,
    void *dst
    )
{
  int status = 0;
  const int32_t * const s = (const int32_t * const)src;
  int32_t *d = (int32_t *)dst;

  if ( src == NULL ) { go_BYE(-1); } 
  if ( dst == NULL ) { go_BYE(-1); } 
  if ( n == 0 ) { go_BYE(-1); }
  int32_t s_right = s[n-1];
  for ( int i = (int)n-1; i >= 0; i-- ) { 
#ifdef DEBUG
    if ( s[i] < 0 ) { go_BYE(-1); } 
#endif
    if ( s[i] == 0 ) { d[i] = 0; s_right = 0; continue; } 
    //-------------------------------
    if ( s_right == 0 ) { s_right = d[i] = s[i]; continue; } 
    d[i] = s_right;
  }
BYE:
  return status;
}
int
flatten_sawtooth_I8(
    const void * const src,
    uint32_t n,
    void *dst
    )
{
  int status = 0;
  const int64_t * const s = (const int64_t * const)src;
  int64_t *d = (int64_t *)dst;

  if ( src == NULL ) { go_BYE(-1); } 
  if ( dst == NULL ) { go_BYE(-1); } 
  if ( n == 0 ) { go_BYE(-1); }
  int64_t s_right = s[n-1];
  for ( int i = (int)n-1; i >= 0; i-- ) { 
#ifdef DEBUG
    if ( s[i] < 0 ) { go_BYE(-1); } 
#endif
    if ( s[i] == 0 ) { d[i] = 0; s_right = 0; continue; } 
    //-------------------------------
    if ( s_right == 0 ) { s_right = d[i] = s[i]; continue; } 
    d[i] = s_right;
  }
BYE:
  return status;
}
