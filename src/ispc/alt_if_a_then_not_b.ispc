export void
alt_if_a_then_not_b(
    uniform uint64 X[], // [n]
    uniform int n,
    uniform int a,
    uniform int b
    )
{
  foreach ( i = 0 ... n  ) {
    uint64 X_i = X[i];
    if ( ( X_i & ((uint64)1 << a) ) != 0 ) { // a is set 
      if ( ( X_i & ((uint64)1 << b) ) != 0 ) { // b is set 
        // unset b 
        uint64 mask = ((uint64)1 << b); // 0 except for b location
        mask = ~mask; // 1 except for b location
        X[i] &= mask;
      }
    }
  }
}
 
