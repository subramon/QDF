// Some simple utilities to help with QDF testing 
#include "q_incs.h"
#include "qtypes.h"
//--- QDF 
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_pr.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_test.h"

// make a simple data frame: useful for testing 
int
qdf_mk_df(
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  uint32_t n_svals = 11; 
  char *svals[n_svals];
  qtype_t qtypes[n_svals];
  uint32_t widths[n_svals];
  char *vals[n_svals];

  uint32_t n_vals = SCHAR_MAX;  // don't exceed this

  // set qtypes 
  qtypes[0] = I1;
  qtypes[1] = I2;
  qtypes[2] = I4;
  qtypes[3] = I8;
  qtypes[4] = UI1;
  qtypes[5] = UI2;
  qtypes[6] = UI4;
  qtypes[7] = UI8;
  qtypes[8] = F4;
  qtypes[9] = F8;
  qtypes[10] = SC;

  // set widths 
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    widths[i] = get_width_c_qtype(qtypes[i]);
  }
  widths[10] = 16; // NOTE hard coded 
  // set names of columns 
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    char buf[8]; sprintf(buf, "fld_%d", i); 
    svals[i] = strdup(buf);
  }
  // malloc vals 
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    vals[i] = malloc(n_vals * widths[i]); 
  }
  // START: Set vals
  // set for signed integers
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((int8_t  *)vals[0])[i] = (int8_t)(i+1);
  }
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((int16_t  *)vals[1])[i] = 10*(int16_t)(i+1);
  }
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((int32_t  *)vals[2])[i] = 100*(int32_t)(i+1);
  }
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((int64_t  *)vals[3])[i] = 1000*(int64_t)(i+1);
  }
  // set for unsigned integers
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((uint8_t  *)vals[4])[i] = (uint8_t)(i+1);
  }
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((uint16_t  *)vals[5])[i] = 10*(uint16_t)(i+1);
  }
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((uint32_t  *)vals[6])[i] = 100*(uint32_t)(i+1);
  }
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((uint64_t  *)vals[7])[i] = 1000*(uint64_t)(i+1);
  }
  // set for floats 
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((float  *)vals[8])[i] = 10000*(float)(i+1);
  }
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((double  *)vals[9])[i] = 10000*(double)(i+1);
  }
  // set for strings
  char *cptr = vals[10]; 
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    strcpy(cptr, "hello world"); 
    cptr += widths[10];
  }
  //----------------------
  status = make_data_frame(svals, n_svals, widths,
      vals, n_vals, 0, qtypes, ptr_qdf);
  cBYE(status);
BYE:
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    free_if_non_null(svals[i]);
  }
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    free_if_non_null(vals[i]);
  }
  return status;
}
