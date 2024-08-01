#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_setters.h"
#include "qdf_f_to_s.h"
#include "qdf_squeeze_where.h"

// Returns number of rows after squeezing.
// A return of < 0 indicates an error 
int
squeeze_where(
    QDF_REC_TYPE *ptr_dst, // INPUT and OUTPUT: df consisting of numeric arrays
    const QDF_REC_TYPE *const where, // numeric I1 array (1=> good, 0 => null)
    uint32_t *ptr_num_good
   )
{
  int status = 0;
  void *buf = NULL; 
  // TODO P2: Important check to enforce, 
  // "where" should not be part of "ptr_dst"
  //==============================
  mcr_chk_non_null(ptr_dst, -1); 
  if ( ptr_dst->is_read_only ) { go_BYE(-1); } 
  mcr_chk_non_null(where, -1); 

  char *wx = where->data; 
  jtype_t wjtype = get_jtype(wx); if ( wjtype != j_array ) {  go_BYE(-1); }
  uint32_t wn    = get_arr_len(wx); 
  qtype_t wqtype = get_qtype(wx); if ( wqtype != I1 ) { go_BYE(-1); } 
  // find out how many items left after squeeze
  SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(where, "sum", NULL, &sclr); cBYE(status);
  double dtemp; status = get_sclr_as_F8(&sclr, &dtemp); cBYE(status);
  uint32_t num_good = (uint32_t)dtemp;
  *ptr_num_good = num_good; 
  // Nothing to squeeze => all elements are "good"
  if ( num_good  == wn ) { return status; } 
  if ( num_good  > wn ) { go_BYE(-1); } 
  //==============================
  const char * const wptr = get_arr_ptr(wx); 

  char *dx = ptr_dst->data; 
  qtype_t dqtype = get_qtype(dx); 
  jtype_t djtype = get_jtype(dx); 
  uint32_t dn = 0;
  if ( djtype == j_object ) {  
    if ( !get_is_df(dx) ) { go_BYE(-1); }
    dn = get_obj_arr_len(dx); 
  }
  else if ( djtype == j_array ) {  
    if ( dqtype == Q0 ) { go_BYE(-1); } 
    dn = get_arr_len(dx); 
  }
  if ( wn != dn ) { go_BYE(-1); } 
  //==============================
  uint32_t max_width = sizeof(double);
  uint32_t num_null = wn - num_good;
  // buf is used to store items that will be moved to the end
  // these are items where the where field is 0 
  buf = malloc(num_null * max_width);
  if ( buf == NULL ) { 
    printf("wn =  %d \n", wn);
    printf("num_good =  %d \n", num_good);
    printf("Unable to alloc %d \n", num_null * max_width);
  }
  return_if_malloc_failed(buf);
  //==============================
  uint32_t dn_keys;
  switch ( djtype ) { 
    case j_array : dn_keys = 1; break; 
    case j_object : dn_keys = get_obj_len(dx); break; 
    default : go_BYE(-1); break;
  }
  for ( uint32_t i = 0; i < dn_keys; i++ ) {  // for each col in df
    char *cx = NULL;
    switch ( djtype ) { 
      case j_array : 
        {
          cx = ptr_dst->data;
        }
        break;
      case j_object : 
        {
          QDF_REC_TYPE col; memset(&col, 0, sizeof(QDF_REC_TYPE));
          status = get_key_val(ptr_dst, (int)i, NULL, &col, NULL); 
          cBYE(status);
          cx = col.data; 
        }
        break;
      default :
        go_BYE(-1);
        break;
    }
    void *valptr    = get_arr_ptr(cx); 
    // important that valptr is "char *" because 
    // of memcpy pointer arithmetic further on 
    qtype_t qtype   = get_qtype(cx);
    uint32_t cwidth = get_arr_width(cx);
    uint32_t cn     = get_arr_len(cx); 
    if ( cn == 0 ) { continue; } // IMPORTANT: for place holder columns
    if ( cwidth == 0 ) { go_BYE(-1); } // Limitation
    if ( cwidth > max_width ) { go_BYE(-1); } // Limitation
    set_arr_len(cx, num_good);  
    //-------------------------------------------
    uint32_t outidx = 0;
    uint32_t bufidx = 0;
    switch ( qtype ) { 
      // TODO P3 This should be auto-generated
      case F4 : 
        for ( uint32_t j = 0; j < cn; j++ ) { 
          if ( wptr[j] == 1 ) { 
            ((float *)valptr)[outidx++] = ((float *)valptr)[j];
          }
          else {
            ((float *)buf)[bufidx++] = ((float *)valptr)[j];
          }
        }
        memcpy((char *)valptr + (num_good * cwidth), buf, num_null * cwidth);
        break;
      case F8 : 
        for ( uint32_t j = 0; j < cn; j++ ) { 
          if ( wptr[j] == 1 ) { 
            ((double *)valptr)[outidx++] = ((double *)valptr)[j];
          }
          else {
            ((double *)buf)[bufidx++] = ((double *)valptr)[j];
          }
        }
        memcpy((char *)valptr + (num_good * cwidth), buf, num_null * cwidth);
        break;
      case I1 : 
        for ( uint32_t j = 0; j < cn; j++ ) { 
          if ( wptr[j] == 1 ) { 
            ((int8_t *)valptr)[outidx++] = ((int8_t *)valptr)[j];
          }
          else {
            ((int8_t *)buf)[bufidx++] = ((int8_t *)valptr)[j];
          }
        }
        memcpy((char *)valptr + (num_good * cwidth), buf, num_null * cwidth);
        break;
      case I2 : 
        for ( uint32_t j = 0; j < cn; j++ ) { 
          if ( wptr[j] == 1 ) { 
            ((int16_t *)valptr)[outidx++] = ((int16_t *)valptr)[j];
          }
          else {
            ((int16_t *)buf)[bufidx++] = ((int16_t *)valptr)[j];
          }
        }
        memcpy((char *)valptr + (num_good * cwidth), buf, num_null * cwidth);
        break;
      case I4 : 
        for ( uint32_t j = 0; j < cn; j++ ) { 
          if ( wptr[j] == 1 ) { 
            ((int32_t *)valptr)[outidx++] = ((int32_t *)valptr)[j];
          }
          else {
            ((int32_t *)buf)[bufidx++] = ((int32_t *)valptr)[j];
          }
        }
        memcpy((char *)valptr + (num_good * cwidth), buf, num_null * cwidth);
        break;
      case TM1 : 
        for ( uint32_t j = 0; j < cn; j++ ) { 
          if ( wptr[j] == 1 ) { 
            ((tm_t *)valptr)[outidx++] = ((tm_t *)valptr)[j];
          }
          else {
            ((tm_t *)buf)[bufidx++] = ((tm_t *)valptr)[j];
          }
        }
        memcpy((char *)valptr + (num_good * cwidth), buf, num_null * cwidth);
        break;
      default : 
        fprintf(stderr, "To be implemented\n"); go_BYE(-1); 
        break;
    }
    if ( outidx != num_good ) { go_BYE(-1); }
    if ( bufidx != num_null ) { go_BYE(-1); }
  }
  if ( djtype == j_object ) { 
    x_set_obj_arr_len(ptr_dst, num_good); // IMPORTANT
  }
BYE:
  free_if_non_null(buf);
  return status; 
}
