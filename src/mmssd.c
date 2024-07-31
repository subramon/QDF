// This is ideal candidate for code generation 
#include "incs.h"
#include "qdf_struct.h"
#include "hash.h"
#include "rs_mmap.h"
#include "auxil.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_mmssd.h" 
// mmssd stands for min, max, sum, standard deviation
int
v_s_to_s(
    QDF_REC_TYPE *src, // input numeric array 
    const char * const lop,
    QDF_SCLR_TYPE *sclr_in, // input 
    QDF_SCLR_TYPE *sclr_out // output 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  //==============================
  if ( src == NULL ) { go_BYE(-1); }
  void *sx = src->data; 
  if ( sx == NULL ) { go_BYE(-1); }
  jtype_t sjtype = get_jtype(sx); 
  if ( sjtype != j_array ) {  go_BYE(-1); }
  uint32_t sn = get_n_elem(sx); 
  uint32_t sz = get_size(sx); 
  qtype_t sqtype = get_qtype(sx); 
  //==============================
  // TODO P3 We are not yet set up to handle type promotion

  if ( strcmp(lop, "min") == 0 ) { 
    switch ( sqtype ) { 
      case F4 : 
        float *X = get_arr_ptr(sx);
        float min_val = X[0];
        for ( int i = 0; i < n; i++ ) { 
          if ( X[i] < min_val ) { min_val = X[i]; }
        }
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  else if ( strcmp(lop, "min") == 0 ) { 
    switch ( sqtype ) { 
      case F4 : 
        float *X = get_arr_ptr(sx);
        float min_val = X[0];
        for ( int i = 0; i < n; i++ ) { 
          if ( X[i] < min_val ) { min_val = X[i]; }
        }
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  else if ( strcmp(lop, "sum") == 0 ) { 
    double sum_val = 0;
    switch ( sqtype ) { 
      case F4 : 
        float *X = get_arr_ptr(sx);
        for ( int i = 0; i < n; i++ ) { 
          sum_val += X[i];
        }
        break;
      default : 
        go_BYE(-1);
        break;
    }
    sclr_out->val.f8 = sum_val; 
    sclr_out->qtype =  F8;
  }
  else if ( strcmp(lop, "sd") == 0 ) { 
    if ( sclr_in->qtype != F8 ) { go_BYE(-1); }
    double mu = sclr_in->val.f8; 
    double sum = 0; 
    switch ( sqtype ) { 
      case F4 : 
        float *X = get_arr_ptr(sx);
        for ( int i = 0; i < n; i++ ) { 
          double diff = X[i] - mu;
          sum += diff * diff; 
        }
        break;
      default : 
        go_BYE(-1);
        break;
    }
    sclr_out->val.f8 = sqrt(sum/n);
    sclr_out->qtype =  F8;
  }
  else {
    go_BYE(-1); 
  }
BYE:
  return status;
}
