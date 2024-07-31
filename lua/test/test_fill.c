#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#define WHEREAMI { fprintf(stderr, "Line %3d of File %s \n", __LINE__, __FILE__);  }
#define go_BYE(x) { WHEREAMI; status = x ; goto BYE; }
#define cBYE(x) { if ( (x) < 0 ) { go_BYE((x)) } }
#include "test_fill.h"

int 
test_fill(
    double junk,
    const float * const pre,
    const int8_t * const nn_pre,
    const float * const post,
    const int16_t * const grp,
    int n
    )
{
  int status = 0;
  bool pr = false;
  for ( int i = 0; i < n; i++ ) {
    // if the pre value was defined, then it must be same as post value
    // In other words, it is unchanged by fill
    if ( nn_pre[i] == 1 ) { 
      if ( pre[i] != post[i] ) { 
        printf("mismatch at position %d, %f != %f \n", i,
            pre[i], post[i]);
        go_BYE(-1); }
      continue; 
    }
    // Control comes here => value of ith position needed to be imputed
    if ( nn_pre[i] != 0 ) { go_BYE(-1); }
    int16_t g_i = grp[i]; 
    bool found = false;
    float post_i = post[i];
    for ( int j = i-1; j >= 0; j-- ) {
      if ( grp[j] != g_i ) { break; }
      if ( nn_pre[j] == 1 ) { 
        if ( post_i != post[j] ) { go_BYE(-1); }
        found = true; break;
      }
    }
    if ( found ) {  break; }
    // control here => back fill failed. Try forward fill 
    for ( int j = i+1; j < n; j++ ) {
      if ( grp[j] != g_i ) { break; }
      if ( nn_pre[j] == 1 ) { 
        if ( post_i != post[j] ) { go_BYE(-1); }
        found = true; break;
      }
    }
    if ( found ) {  break; }
    // control here => forward fill failed. Try averaging 
    if ( pr == false )  { 
      printf("need to write averaging test code\n");
      pr = true;
    }
  }
BYE:
  return status;
}
