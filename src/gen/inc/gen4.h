#include "incs.h"
#include "qtypes.h"
extern void
seq_I1(
    int8_t start,
    int8_t incr,
    uint32_t n,
    int8_t * restrict out
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
seq_I2(
    int16_t start,
    int16_t incr,
    uint32_t n,
    int16_t * restrict out
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
seq_I4(
    int32_t start,
    int32_t incr,
    uint32_t n,
    int32_t * restrict out
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
seq_I8(
    int64_t start,
    int64_t incr,
    uint32_t n,
    int64_t * restrict out
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
seq_F4(
    float start,
    float incr,
    uint32_t n,
    float * restrict out
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
seq_F8(
    double start,
    double incr,
    uint32_t n,
    double * restrict out
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
incr_on_by_I1(
    const int8_t *const invals,
    int8_t on,
    int8_t incr,
    bool reset,
    uint32_t n,
    int8_t *restrict outvals
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
incr_on_by_I2(
    const int16_t *const invals,
    int16_t on,
    int16_t incr,
    bool reset,
    uint32_t n,
    int16_t *restrict outvals
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
incr_on_by_I4(
    const int32_t *const invals,
    int32_t on,
    int32_t incr,
    bool reset,
    uint32_t n,
    int32_t *restrict outvals
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
incr_on_by_I8(
    const int64_t *const invals,
    int64_t on,
    int64_t incr,
    bool reset,
    uint32_t n,
    int64_t *restrict outvals
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
incr_on_by_F4(
    const float *const invals,
    float on,
    float incr,
    bool reset,
    uint32_t n,
    float *restrict outvals
    );
    
#include "incs.h"
#include "qtypes.h"
extern void
incr_on_by_F8(
    const double *const invals,
    double on,
    double incr,
    bool reset,
    uint32_t n,
    double *restrict outvals
    );
    
#include "incs.h"
#include "qtypes.h"
extern bool
is_sawtooth_I1(
    const int8_t *const invals,
    uint32_t n
    );
    
#include "incs.h"
#include "qtypes.h"
extern bool
is_sawtooth_I2(
    const int16_t *const invals,
    uint32_t n
    );
    
#include "incs.h"
#include "qtypes.h"
extern bool
is_sawtooth_I4(
    const int32_t *const invals,
    uint32_t n
    );
    
#include "incs.h"
#include "qtypes.h"
extern bool
is_sawtooth_I8(
    const int64_t *const invals,
    uint32_t n
    );
    
#include "incs.h"
#include "qtypes.h"
extern bool
is_sawtooth_F4(
    const float *const invals,
    uint32_t n
    );
    
#include "incs.h"
#include "qtypes.h"
extern bool
is_sawtooth_F8(
    const double *const invals,
    uint32_t n
    );
    
