#include "vvrem.h" 
int
vvrem(
    char *x,
    const char * const y,
    uint32_t n,
    qtype_t xqtype,
    char *z
    )
{
  int status = 0; 

  switch ( xqtype ) { 
    case I1 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = (int8_t)(((int8_t *)x)[i]  % 
        ((const int8_t * const)y)[i]);
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int16_t *)z)[i] = (int16_t)(((int16_t *)x)[i]  %  ((const int16_t * const)y)[i]);
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int32_t *)z)[i] = (int32_t)(((int32_t *)x)[i]  %  ((const int32_t * const)y)[i]);
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int64_t *)z)[i] = (int64_t)(((int64_t *)x)[i]  %  ((const int64_t * const)y)[i]);
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvbitwise_and.h" 
int
vvbitwise_and(
    char *x,
    const char * const y,
    uint32_t n,
    qtype_t xqtype,
    char *z
    )
{
  int status = 0; 

  switch ( xqtype ) { 
    case I1 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = (int8_t)(((int8_t *)x)[i]  & 
        ((const int8_t * const)y)[i]);
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int16_t *)z)[i] = (int16_t)(((int16_t *)x)[i]  &  ((const int16_t * const)y)[i]);
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int32_t *)z)[i] = (int32_t)(((int32_t *)x)[i]  &  ((const int32_t * const)y)[i]);
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int64_t *)z)[i] = (int64_t)(((int64_t *)x)[i]  &  ((const int64_t * const)y)[i]);
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvbitwise_or.h" 
int
vvbitwise_or(
    char *x,
    const char * const y,
    uint32_t n,
    qtype_t xqtype,
    char *z
    )
{
  int status = 0; 

  switch ( xqtype ) { 
    case I1 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = (int8_t)(((int8_t *)x)[i]  | 
        ((const int8_t * const)y)[i]);
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int16_t *)z)[i] = (int16_t)(((int16_t *)x)[i]  |  ((const int16_t * const)y)[i]);
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int32_t *)z)[i] = (int32_t)(((int32_t *)x)[i]  |  ((const int32_t * const)y)[i]);
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int64_t *)z)[i] = (int64_t)(((int64_t *)x)[i]  |  ((const int64_t * const)y)[i]);
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvbitwise_xor.h" 
int
vvbitwise_xor(
    char *x,
    const char * const y,
    uint32_t n,
    qtype_t xqtype,
    char *z
    )
{
  int status = 0; 

  switch ( xqtype ) { 
    case I1 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = (int8_t)(((int8_t *)x)[i]  ^ 
        ((const int8_t * const)y)[i]);
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int16_t *)z)[i] = (int16_t)(((int16_t *)x)[i]  ^  ((const int16_t * const)y)[i]);
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int32_t *)z)[i] = (int32_t)(((int32_t *)x)[i]  ^  ((const int32_t * const)y)[i]);
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int64_t *)z)[i] = (int64_t)(((int64_t *)x)[i]  ^  ((const int64_t * const)y)[i]);
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
