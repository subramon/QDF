#ifndef __QDF_MACROS_H
#define __QDF_MACROS_H

#define mcr_chk_non_null(x, y) {  \
  if ( x == NULL ) { WHEREAMI; return y; } \
  if ( x->data == NULL ) { WHEREAMI; return y; } \
  if ( x->size == 0 ) { WHEREAMI; return y; } \
}
#define mcr_chk_null(x, y) {  \
  if ( x == NULL ) { WHEREAMI; return y; } \
  if ( x->data != NULL ) { WHEREAMI; return  y; } \
  if ( x->name != NULL ) { WHEREAMI; return y; } \
  if ( x->size != 0 ) { WHEREAMI; return y; } \
  if ( ( x->is_foreign != 0 )  && ( x->is_foreign != 1 ) ) { WHEREAMI; return y; } \
  if ( ( x->is_mmap != 0 )  && ( x->is_mmap != 1 ) ) { WHEREAMI; return y; } \
  if ( ( x->is_read_only != 0 )  && ( x->is_read_only != 1 ) ) { WHEREAMI; return y; } \
}

#endif
