#include <stdint.h>
#include "if_a_then_not_b.h"
typedef int holiday_enum_t;
void
if_a_then_not_b(
    uint64_t *X, // [n]
    int n,
    holiday_enum_t a,
    holiday_enum_t b
    )
{
  for ( int i = 0; i < n; i++ ) { 
    uint64_t X_i = X[i];
    if ( ( X_i & ((uint64_t)1 << a) ) != 0 ) { // a is set 
      if ( ( X_i & ((uint64_t)1 << b) ) != 0 ) { // b is set 
        // unset b 
        uint64_t mask = ((uint64_t)1 << b); // 0 except for b location
        mask = ~mask; // 1 except for b location
        X[i] &= mask;
      }
    }
  }
}
 
