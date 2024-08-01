#include "vseq_I1.h" 
void
vseq_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vseq_I2.h" 
void
vseq_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vseq_I4.h" 
void
vseq_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vseq_I8.h" 
void
vseq_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vseq_UI1.h" 
void
vseq_UI1(
    uint8_t *x,
    uint8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vseq_UI2.h" 
void
vseq_UI2(
    uint16_t *x,
    uint16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vseq_UI4.h" 
void
vseq_UI4(
    uint32_t *x,
    uint32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vseq_UI8.h" 
void
vseq_UI8(
    uint64_t *x,
    uint64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vseq_F4.h" 
void
vseq_F4(
    float *x,
    float sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vseq_F8.h" 
void
vseq_F8(
    double *x,
    double sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] == sclr); 
  }
}
    
#include "vsneq_I1.h" 
void
vsneq_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vsneq_I2.h" 
void
vsneq_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vsneq_I4.h" 
void
vsneq_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vsneq_I8.h" 
void
vsneq_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vsneq_UI1.h" 
void
vsneq_UI1(
    uint8_t *x,
    uint8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vsneq_UI2.h" 
void
vsneq_UI2(
    uint16_t *x,
    uint16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vsneq_UI4.h" 
void
vsneq_UI4(
    uint32_t *x,
    uint32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vsneq_UI8.h" 
void
vsneq_UI8(
    uint64_t *x,
    uint64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vsneq_F4.h" 
void
vsneq_F4(
    float *x,
    float sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vsneq_F8.h" 
void
vsneq_F8(
    double *x,
    double sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] != sclr); 
  }
}
    
#include "vslt_I1.h" 
void
vslt_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vslt_I2.h" 
void
vslt_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vslt_I4.h" 
void
vslt_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vslt_I8.h" 
void
vslt_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vslt_UI1.h" 
void
vslt_UI1(
    uint8_t *x,
    uint8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vslt_UI2.h" 
void
vslt_UI2(
    uint16_t *x,
    uint16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vslt_UI4.h" 
void
vslt_UI4(
    uint32_t *x,
    uint32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vslt_UI8.h" 
void
vslt_UI8(
    uint64_t *x,
    uint64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vslt_F4.h" 
void
vslt_F4(
    float *x,
    float sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vslt_F8.h" 
void
vslt_F8(
    double *x,
    double sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] < sclr); 
  }
}
    
#include "vsleq_I1.h" 
void
vsleq_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsleq_I2.h" 
void
vsleq_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsleq_I4.h" 
void
vsleq_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsleq_I8.h" 
void
vsleq_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsleq_UI1.h" 
void
vsleq_UI1(
    uint8_t *x,
    uint8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsleq_UI2.h" 
void
vsleq_UI2(
    uint16_t *x,
    uint16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsleq_UI4.h" 
void
vsleq_UI4(
    uint32_t *x,
    uint32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsleq_UI8.h" 
void
vsleq_UI8(
    uint64_t *x,
    uint64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsleq_F4.h" 
void
vsleq_F4(
    float *x,
    float sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsleq_F8.h" 
void
vsleq_F8(
    double *x,
    double sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] <= sclr); 
  }
}
    
#include "vsgt_I1.h" 
void
vsgt_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgt_I2.h" 
void
vsgt_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgt_I4.h" 
void
vsgt_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgt_I8.h" 
void
vsgt_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgt_UI1.h" 
void
vsgt_UI1(
    uint8_t *x,
    uint8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgt_UI2.h" 
void
vsgt_UI2(
    uint16_t *x,
    uint16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgt_UI4.h" 
void
vsgt_UI4(
    uint32_t *x,
    uint32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgt_UI8.h" 
void
vsgt_UI8(
    uint64_t *x,
    uint64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgt_F4.h" 
void
vsgt_F4(
    float *x,
    float sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgt_F8.h" 
void
vsgt_F8(
    double *x,
    double sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] > sclr); 
  }
}
    
#include "vsgeq_I1.h" 
void
vsgeq_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsgeq_I2.h" 
void
vsgeq_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsgeq_I4.h" 
void
vsgeq_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsgeq_I8.h" 
void
vsgeq_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsgeq_UI1.h" 
void
vsgeq_UI1(
    uint8_t *x,
    uint8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsgeq_UI2.h" 
void
vsgeq_UI2(
    uint16_t *x,
    uint16_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsgeq_UI4.h" 
void
vsgeq_UI4(
    uint32_t *x,
    uint32_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsgeq_UI8.h" 
void
vsgeq_UI8(
    uint64_t *x,
    uint64_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsgeq_F4.h" 
void
vsgeq_F4(
    float *x,
    float sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsgeq_F8.h" 
void
vsgeq_F8(
    double *x,
    double sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] >= sclr); 
  }
}
    
#include "vsrem_I1.h" 
void
vsrem_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] % sclr); 
  }
}
    
#include "vsrem_I2.h" 
void
vsrem_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int16_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int16_t)(x[i] % sclr); 
  }
}
    
#include "vsrem_I4.h" 
void
vsrem_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int32_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int32_t)(x[i] % sclr); 
  }
}
    
#include "vsrem_I8.h" 
void
vsrem_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int64_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int64_t)(x[i] % sclr); 
  }
}
    
#include "vsrem_UI1.h" 
void
vsrem_UI1(
    uint8_t *x,
    uint8_t sclr,
    uint32_t n,
    uint8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (uint8_t)(x[i] % sclr); 
  }
}
    
#include "vsrem_UI2.h" 
void
vsrem_UI2(
    uint16_t *x,
    uint16_t sclr,
    uint32_t n,
    uint16_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (uint16_t)(x[i] % sclr); 
  }
}
    
#include "vsrem_UI4.h" 
void
vsrem_UI4(
    uint32_t *x,
    uint32_t sclr,
    uint32_t n,
    uint32_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (uint32_t)(x[i] % sclr); 
  }
}
    
#include "vsrem_UI8.h" 
void
vsrem_UI8(
    uint64_t *x,
    uint64_t sclr,
    uint32_t n,
    uint64_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (uint64_t)(x[i] % sclr); 
  }
}
    
#include "vsbitwise_and_I1.h" 
void
vsbitwise_and_I1(
    uint8_t *x,
    uint8_t sclr,
    uint32_t n,
    uint8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (uint8_t)(x[i] & sclr); 
  }
}
    
#include "vsbitwise_and_I2.h" 
void
vsbitwise_and_I2(
    uint16_t *x,
    uint16_t sclr,
    uint32_t n,
    uint16_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (uint16_t)(x[i] & sclr); 
  }
}
    
#include "vsbitwise_and_I4.h" 
void
vsbitwise_and_I4(
    uint32_t *x,
    uint32_t sclr,
    uint32_t n,
    uint32_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (uint32_t)(x[i] & sclr); 
  }
}
    
#include "vsbitwise_and_I8.h" 
void
vsbitwise_and_I8(
    uint64_t *x,
    uint64_t sclr,
    uint32_t n,
    uint64_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (uint64_t)(x[i] & sclr); 
  }
}
    
