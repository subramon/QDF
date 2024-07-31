#include "vvmin.h" 
int
vvmin(
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
        ((int8_t *)z)[i] = 
        ((int8_t *)x)[i]  < 
        ((const int8_t * const)y)[i] ? 
        ((int8_t *)x)[i] : ((const int8_t * const)y)[i] ;
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int16_t *)z)[i] = 
        ((int16_t *)x)[i]  < 
        ((const int16_t * const)y)[i] ? 
        ((int16_t *)x)[i] : ((const int16_t * const)y)[i] ;
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int32_t *)z)[i] = 
        ((int32_t *)x)[i]  < 
        ((const int32_t * const)y)[i] ? 
        ((int32_t *)x)[i] : ((const int32_t * const)y)[i] ;
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int64_t *)z)[i] = 
        ((int64_t *)x)[i]  < 
        ((const int64_t * const)y)[i] ? 
        ((int64_t *)x)[i] : ((const int64_t * const)y)[i] ;
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((float *)z)[i] = 
        ((float *)x)[i]  < 
        ((const float * const)y)[i] ? 
        ((float *)x)[i] : ((const float * const)y)[i] ;
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((double *)z)[i] = 
        ((double *)x)[i]  < 
        ((const double * const)y)[i] ? 
        ((double *)x)[i] : ((const double * const)y)[i] ;
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvmax.h" 
int
vvmax(
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
        ((int8_t *)z)[i] = 
        ((int8_t *)x)[i]  > 
        ((const int8_t * const)y)[i] ? 
        ((int8_t *)x)[i] : ((const int8_t * const)y)[i] ;
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int16_t *)z)[i] = 
        ((int16_t *)x)[i]  > 
        ((const int16_t * const)y)[i] ? 
        ((int16_t *)x)[i] : ((const int16_t * const)y)[i] ;
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int32_t *)z)[i] = 
        ((int32_t *)x)[i]  > 
        ((const int32_t * const)y)[i] ? 
        ((int32_t *)x)[i] : ((const int32_t * const)y)[i] ;
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int64_t *)z)[i] = 
        ((int64_t *)x)[i]  > 
        ((const int64_t * const)y)[i] ? 
        ((int64_t *)x)[i] : ((const int64_t * const)y)[i] ;
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((float *)z)[i] = 
        ((float *)x)[i]  > 
        ((const float * const)y)[i] ? 
        ((float *)x)[i] : ((const float * const)y)[i] ;
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((double *)z)[i] = 
        ((double *)x)[i]  > 
        ((const double * const)y)[i] ? 
        ((double *)x)[i] : ((const double * const)y)[i] ;
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
