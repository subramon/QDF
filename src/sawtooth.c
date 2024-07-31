#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "sawtooth.h"
// Following is candidate for code generation
int
sawtooth(
    const char *const src,
    uint32_t n,
    qtype_t qtype,
    char *dst
    )
{
  int status = 0;
  switch ( qtype ) { 
    case I1 : sawtooth_I1(src, n, dst); cBYE(status); break;
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
// Following is candidate for code generation
// TODO P4 Allow differnet types of src and dst 
int
sawtooth_I1(
    const char *const src,
    uint32_t n,
    char *dst
    )
{
  int status = 0;
  const int8_t *const s = (const int8_t *const )src;
  int8_t *d = (int8_t *)dst;

  if ( src == NULL ) { go_BYE(-1); } 
  if ( dst == NULL ) { go_BYE(-1); } 
  if ( n == 0 ) { go_BYE(-1); }
  int8_t prev_d = 0;
  for ( uint32_t i = 0; i < n; i++ ) { 
#ifdef DEBUG
    if ( s[i] < 0 ) { go_BYE(-1); } 
#endif
    if ( s[i] == 0 ) { 
      d[i] = 0;
    }
    else {
      d[i] = (int8_t)(prev_d + s[i]);
    }
    prev_d = d[i];
  }
  /* Decided against doing this 
  // subtract 1 from non-zero's
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( d[i] > 0 ) { d[i]--; }
  }
  */
BYE:
  return status;
}
