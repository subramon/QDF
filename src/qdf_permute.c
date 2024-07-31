#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_permute.h"
int
permute(
    const QDF_REC_TYPE * const src, 
    const QDF_REC_TYPE * const idx, 
    const char * const direction,
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  bool *idx_seen = NULL;

  mcr_chk_non_null(src, -1); 
  void *sx = src->data; 
  uint32_t sn     = get_arr_len(sx); 
  jtype_t sjtype = get_jtype(sx);   if ( sjtype != j_array ) { go_BYE(-1); }
  qtype_t sqtype = get_qtype(sx); 
  const char * const sptr = get_arr_ptr(sx); 
  uint32_t swidth = get_arr_width(sx);   if ( swidth == 0 ) { go_BYE(-1); }
  //==============================
  mcr_chk_non_null(idx, -1); 
  void *ix = idx->data; 
  uint32_t in     = get_arr_len(ix); 
  jtype_t ijtype = get_jtype(ix); if ( ijtype != j_array ) { go_BYE(-1); }
  qtype_t iqtype = get_qtype(ix); 
  const void * const iptr = get_arr_ptr(ix); 
  //==============================
#ifdef DEBUG
  idx_seen = malloc(in * sizeof(bool));
  memset(idx_seen, 0, (in * sizeof(bool)));
  for ( uint32_t i = 0; i < in; i++ ) { 
    int idxval = ((const int32_t * const)iptr)[i]; if ( ( idxval < 0 ) || ( idxval >= (int)in ) ) { go_BYE(-1); } 
    if ( idx_seen[idxval] ) { go_BYE(-1); } // no dupes
    idx_seen[idxval] = true;
  }
#endif
  if ( sn != in ) { go_BYE(-1); }
  //==============================
  status = make_num_array(NULL, sn, 0, sqtype,  dst); cBYE(status);
  char *dptr = get_arr_ptr(dst->data); 
  if ( strcmp(direction, "forward") == 0 ) {
    for ( uint32_t toidx = 0; toidx < sn; toidx++ ) { 
      int32_t ifromidx;
      switch ( iqtype ) { 
        case I1 : 
          ifromidx = (int32_t)(((const int8_t * const )iptr)[toidx]);
          break; 
        case I2 : 
          ifromidx = (int32_t)(((const int16_t * const )iptr)[toidx]);
          break; 
        case I4 : 
          ifromidx = (int32_t)(((const int32_t * const )iptr)[toidx]);
          break; 
        case I8 : 
          ifromidx = (int32_t)(((const int64_t * const )iptr)[toidx]);
          break; 
        default : 
          go_BYE(-1);
          break;
      }
#ifdef DEBUG
      if ( ( ifromidx < 0 ) || ( ifromidx >= (int32_t)sn ) ) { go_BYE(-1); }
#endif
      uint32_t ufromidx = (uint32_t)ifromidx;
      memcpy(dptr + (toidx*swidth), sptr + (ufromidx*swidth), swidth);
    }
  }
  else if ( strcmp(direction, "reverse") == 0 ) {
    for ( uint32_t fromidx = 0; fromidx < sn; fromidx++ ) { 
      int32_t itoidx;
      switch ( iqtype ) { 
        case I1 :
          itoidx = (int32_t)(((const int8_t * const )iptr)[fromidx]);
          break; 
        case I2 : 
          itoidx = (int32_t)(((const int16_t * const )iptr)[fromidx]);
          break; 
        case I4 : 
          itoidx = (int32_t)(((const int32_t * const )iptr)[fromidx]);
          break; 
        case I8 : 
          itoidx = (int32_t)(((const int64_t * const )iptr)[fromidx]);
          break; 
        default : 
          go_BYE(-1);
          break;
      }
#ifdef DEBUG
      if ( ( itoidx < 0 ) || ( itoidx >= (int32_t)sn ) ) { go_BYE(-1); }
#endif
      uint32_t utoidx = (uint32_t)itoidx;
      memcpy(dptr + (utoidx*swidth), sptr + (fromidx*swidth), swidth);
    }
  }
  else {
    go_BYE(-1);
  }
BYE:
  free_if_non_null(idx_seen);
  return status;
}
