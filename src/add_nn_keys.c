#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "q_macros.h"
#include "qtypes.h"

#include "rs_strcat.h"
#include "add_nn_keys.h"

// suppose
// keys   = { "foo", "bar, "foobar", "barfoo", }
// qtypes = { "I4", "SC", "F8", "I4", }
// is_load = { true, true, true, false, }
// has_nulls = { false, true, false, false, }
// nK     = 4
// all_keys   = { "foo", "bar, "foobar", "nn_bar", }
// all_qtypes = { "I4", "SC", "F8", "nn", }
// all_nK     = 4 
int
add_nn_keys(
    char **keys, // [nK]
    const qtype_t *const qtypes, // [nK]
    const uint32_t * const widths, // nK] 
    const bool * const has_nulls, // [nK]
    const bool * const is_load, // [nK]
    uint32_t nK,
    // output below 
    char ***ptr_all_keys, // [all_nK]
    qtype_t **ptr_all_qtypes, // [all_nK]
    uint32_t **ptr_all_widths, // [all_nK]
    uint32_t *ptr_all_nK
    )
{
  int status = 0;
  char **all_keys = NULL; 
  qtype_t *all_qtypes = NULL; 
  uint32_t *all_widths = NULL; 
  uint32_t all_nK = 0;

  if ( nK == 0 ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < nK; i++ ) {
    if ( ( is_load != NULL ) && ( is_load[i] == false )) { continue; }
    all_nK++;
    if ( has_nulls[i] ) { all_nK++; }
  }

  all_keys = malloc(all_nK * sizeof(char *));
  memset(all_keys, 0,  all_nK * sizeof(char *));

  all_qtypes = malloc(all_nK * sizeof(qtype_t));
  memset(all_qtypes, 0,  all_nK * sizeof(qtype_t));

  all_widths = malloc(all_nK * sizeof(uint32_t));
  memset(all_widths, 0,  all_nK * sizeof(uint32_t));

  uint32_t inidx = 0, outidx = 0;
  for ( ; inidx < nK; inidx++ ) { 
    if ( ( is_load != NULL ) && ( is_load[inidx] == false )) { continue; }
    all_keys[outidx]   = strdup(keys[inidx]);
    all_qtypes[outidx] = qtypes[inidx];
    all_widths[outidx] = widths[inidx];
    outidx++;
    if ( has_nulls[inidx] ) { 
      if ( outidx >= all_nK ) { go_BYE(-1); }
      all_keys[outidx]   = rs_strcat("nn_", keys[inidx]);
      all_qtypes[outidx] = BL;
      all_widths[outidx] = sizeof(bool);
      outidx++;
    }
  }
  for ( uint32_t i = 0; i < all_nK; i++ ) { 
    if ( all_widths[i] == 0 ) { go_BYE(-1); }
    if ( all_qtypes[i] == Q0 ) { go_BYE(-1); }
  }
  *ptr_all_keys   = all_keys;
  *ptr_all_qtypes = all_qtypes;
  *ptr_all_widths = all_widths;
  *ptr_all_nK     = all_nK;
BYE:
  return status;
}
