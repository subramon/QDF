#include "vvneq.h"
int
vvneq(
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
        ((int8_t *)z)[i] = ((int8_t *)x)[i]  !=  
        ((const int8_t * const)y)[i];
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int16_t *)x)[i]  !=  
        ((const int16_t * const)y)[i];
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int32_t *)x)[i]  !=  
        ((const int32_t * const)y)[i];
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int64_t *)x)[i]  !=  
        ((const int64_t * const)y)[i];
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((float *)x)[i]  !=  
        ((const float * const)y)[i];
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((double *)x)[i]  !=  
        ((const double * const)y)[i];
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vveq.h"
int
vveq(
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
        ((int8_t *)z)[i] = ((int8_t *)x)[i]  ==  
        ((const int8_t * const)y)[i];
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int16_t *)x)[i]  ==  
        ((const int16_t * const)y)[i];
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int32_t *)x)[i]  ==  
        ((const int32_t * const)y)[i];
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int64_t *)x)[i]  ==  
        ((const int64_t * const)y)[i];
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((float *)x)[i]  ==  
        ((const float * const)y)[i];
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((double *)x)[i]  ==  
        ((const double * const)y)[i];
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvlt.h"
int
vvlt(
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
        ((int8_t *)z)[i] = ((int8_t *)x)[i]  <   
        ((const int8_t * const)y)[i];
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int16_t *)x)[i]  <   
        ((const int16_t * const)y)[i];
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int32_t *)x)[i]  <   
        ((const int32_t * const)y)[i];
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int64_t *)x)[i]  <   
        ((const int64_t * const)y)[i];
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((float *)x)[i]  <   
        ((const float * const)y)[i];
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((double *)x)[i]  <   
        ((const double * const)y)[i];
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvleq.h"
int
vvleq(
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
        ((int8_t *)z)[i] = ((int8_t *)x)[i]  <=  
        ((const int8_t * const)y)[i];
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int16_t *)x)[i]  <=  
        ((const int16_t * const)y)[i];
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int32_t *)x)[i]  <=  
        ((const int32_t * const)y)[i];
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int64_t *)x)[i]  <=  
        ((const int64_t * const)y)[i];
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((float *)x)[i]  <=  
        ((const float * const)y)[i];
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((double *)x)[i]  <=  
        ((const double * const)y)[i];
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvgt.h"
int
vvgt(
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
        ((int8_t *)z)[i] = ((int8_t *)x)[i]  >   
        ((const int8_t * const)y)[i];
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int16_t *)x)[i]  >   
        ((const int16_t * const)y)[i];
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int32_t *)x)[i]  >   
        ((const int32_t * const)y)[i];
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int64_t *)x)[i]  >   
        ((const int64_t * const)y)[i];
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((float *)x)[i]  >   
        ((const float * const)y)[i];
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((double *)x)[i]  >   
        ((const double * const)y)[i];
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvgeq.h"
int
vvgeq(
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
        ((int8_t *)z)[i] = ((int8_t *)x)[i]  >=  
        ((const int8_t * const)y)[i];
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int16_t *)x)[i]  >=  
        ((const int16_t * const)y)[i];
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int32_t *)x)[i]  >=  
        ((const int32_t * const)y)[i];
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int64_t *)x)[i]  >=  
        ((const int64_t * const)y)[i];
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((float *)x)[i]  >=  
        ((const float * const)y)[i];
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((double *)x)[i]  >=  
        ((const double * const)y)[i];
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvlogical_and.h"
int
vvlogical_and(
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
        ((int8_t *)z)[i] = ((int8_t *)x)[i]  &&  
        ((const int8_t * const)y)[i];
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int16_t *)x)[i]  &&  
        ((const int16_t * const)y)[i];
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int32_t *)x)[i]  &&  
        ((const int32_t * const)y)[i];
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int64_t *)x)[i]  &&  
        ((const int64_t * const)y)[i];
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((float *)x)[i]  &&  
        ((const float * const)y)[i];
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((double *)x)[i]  &&  
        ((const double * const)y)[i];
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
#include "vvlogical_or.h"
int
vvlogical_or(
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
        ((int8_t *)z)[i] = ((int8_t *)x)[i]  ||  
        ((const int8_t * const)y)[i];
      }
      break;
    case I2 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int16_t *)x)[i]  ||  
        ((const int16_t * const)y)[i];
      }
      break;
    case I4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int32_t *)x)[i]  ||  
        ((const int32_t * const)y)[i];
      }
      break;
    case I8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((int64_t *)x)[i]  ||  
        ((const int64_t * const)y)[i];
      }
      break;
    case F4 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((float *)x)[i]  ||  
        ((const float * const)y)[i];
      }
      break;
    case F8 :
      for ( uint32_t i = 0; i < n; i++ ) {
        ((int8_t *)z)[i] = ((double *)x)[i]  ||  
        ((const double * const)y)[i];
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
