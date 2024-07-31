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
    
