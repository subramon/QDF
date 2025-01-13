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
  bool *all_qtypes = NULL; 
  uint32_t *all_widths = NULL; 
  uint32_t all_nK = 0;

  if ( nK == 0 ) { go_BYE(-1); }
  uint32_t n = 0;
  for ( uint32_t i = 0; i < nK; i++ ) {
    if ( !is_load[i] ) { continue; }
    if ( has_nulls[i] ) { n++; }
  }
  all_nK = nK + n;

  all_keys = malloc(all_nK * sizeof(char *));
  memset(all_keys, 0,  all_nK * sizeof(char *));

  all_qtypes = malloc(all_nK * sizeof(bool));
  memset(all_qtypes, 0,  all_nK * sizeof(bool));

  all_widths = malloc(all_nK * sizeof(uint32_t));
  memset(all_widths, 0,  all_nK * sizeof(bool));

  for ( uint32_t i = 0; i < nK; i++ ) {
    all_keys[i]   = strdup(keys[i]);
    all_qtypes[i] = qtypes[i];
    all_widths[i] = widths[i];
  }
  uint32_t j = nK;
  for ( uint32_t i = 0; i < nK; i++ ) {
    if ( !is_load[i] ) { continue; }
    if ( has_nulls[i] ) { 
      if ( j >= all_nK ) { go_BYE(-1); }
      all_keys[j]   = rs_strcat("nn_", keys[i]);
      all_qtypes[j] = BL;
      all_widths[j] = sizeof(bool);
      j++;
    }
  }
  *ptr_all_keys   = all_keys;
  *ptr_all_qtypes = all_qtypes;
  *ptr_all_widths = all_widths;
  *ptr_all_nK     = all_nK;
BYE:
  return status;
}
