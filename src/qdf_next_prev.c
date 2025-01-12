#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_next_prev.h"

static next_prev_t
get_next_prev(
    const char * const str_op
    )
{
  if ( strcmp(str_op, "is_next")  == 0 ) { return is_next; } 
  if ( strcmp(str_op, "is_prev")  == 0 ) { return is_prev; } 
  return next_prev_undef;
}

static cmp_t
get_cmp(
    const char * const str_cmp
    )
{
  if ( strcmp(str_cmp, "==")  == 0 ) { return cmp_eq; } 
  if ( strcmp(str_cmp, "!=")  == 0 ) { return cmp_neq; } 
  if ( strcmp(str_cmp, ">=")  == 0 ) { return cmp_geq; } 
  if ( strcmp(str_cmp, ">")   == 0 ) { return cmp_gt; } 
  if ( strcmp(str_cmp, "<=")  == 0 ) { return cmp_leq; } 
  if ( strcmp(str_cmp, "<")   == 0 ) { return cmp_lt; } 
  return cmp_undef;
}

int
qdf_next_prev(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const str_op,
    const char * const str_cmp,
    bool *ptr_rslt
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_qdf, -1); 
  next_prev_t op = get_next_prev(str_op);
  if ( op == next_prev_undef ) { go_BYE(-1); } 
  cmp_t cmp = get_cmp(str_cmp);
  if ( cmp == cmp_undef ) { go_BYE(-1); } 

  char * const x = ptr_qdf->data;
  char * y = get_arr_ptr(x); 

  jtype_t jtype = get_jtype(x);
  if ( jtype != j_array ) { go_BYE(-1); }

  qtype_t qtype = get_qtype(x); 
  if ( qtype == Q0 ) { go_BYE(-1); }

  uint32_t n = get_arr_len(x); 
  switch ( op )  {
    case is_next : 
      *ptr_rslt = calc_is_next(y, n, qtype, cmp); 
      break;
    case is_prev : 
      // TODO *ptr_rslt = calc_is_prev(y, n, qtype, str_cmp); 
      go_BYE(-1); 
      break;
    default :
      go_BYE(-1);
      break;
  }
  //--------------------------------------------------
BYE:
  return status;
}

// Candidate for code generation
bool
calc_is_next(
    const void * const X, 
    uint32_t n, 
    qtype_t qtype, 
    cmp_t cmp
    )
{
  switch ( qtype ) {
    case I1 : 
      {
        const int8_t * const I1ptr = (const int8_t * const)X;
        switch ( cmp ) {
          case cmp_eq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I1ptr[i] ==  I1ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_neq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I1ptr[i] != I1ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_lt : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I1ptr[i] <  I1ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_leq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I1ptr[i] <= I1ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_gt : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I1ptr[i] >  I1ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_geq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I1ptr[i] >= I1ptr[i-1] ) { continue; }
              return false;
            }
            break;
          default :
            WHEREAMI; return false;
            break;
        }
      }
      break;
    case I2 : 
      {
        const int16_t * const I2ptr = (const int16_t * const)X;
        switch ( cmp ) {
          case cmp_eq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I2ptr[i] ==  I2ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_neq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I2ptr[i] != I2ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_lt : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I2ptr[i] <  I2ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_leq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I2ptr[i] <= I2ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_gt : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I2ptr[i] >  I2ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_geq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I2ptr[i] >= I2ptr[i-1] ) { continue; }
              return false;
            }
            break;
          default :
            WHEREAMI; return false;
            break;
        }
      }
      break;
    case I4 : 
      {
        const int32_t * const I4ptr = (const int32_t * const)X;
        switch ( cmp ) {
          case cmp_eq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I4ptr[i] ==  I4ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_neq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I4ptr[i] != I4ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_lt : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I4ptr[i] <  I4ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_leq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I4ptr[i] <= I4ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_gt : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I4ptr[i] >  I4ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_geq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I4ptr[i] >= I4ptr[i-1] ) { continue; }
              return false;
            }
            break;
          default :
            WHEREAMI; return false;
            break;
        }
      }
      break;
    case I8 : 
      {
        const int64_t * const I8ptr = (const int64_t * const)X;
        switch ( cmp ) {
          case cmp_eq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I8ptr[i] ==  I8ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_neq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I8ptr[i] != I8ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_lt : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I8ptr[i] <  I8ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_leq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I8ptr[i] <= I8ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_gt : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I8ptr[i] >  I8ptr[i-1] ) { continue; }
              return false;
            }
            break;
          case cmp_geq : 
            for ( uint32_t i = 1; i < n; i++ ) {
              if ( I8ptr[i] >= I8ptr[i-1] ) { continue; }
              return false;
            }
            break;
          default :
            WHEREAMI; return false;
            break;
        }
      }
      break;
    default :
            WHEREAMI; return false;
      
      break;
  }
  return true;
}
