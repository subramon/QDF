#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "tm2time.h"
#include "qdf_tm.h"

int
tm_extract(
    const QDF_REC_TYPE *const ptr_src, // INPUT 
    const char * const fld, // INPUT 
    QDF_REC_TYPE * restrict ptr_dst // OUTPUT 
    )
{
  int status = 0;

  mcr_chk_non_null(ptr_src, -1); 
  mcr_chk_null(ptr_dst, -1); 
  char *sx = ptr_src->data;
  jtype_t sjtype = get_jtype(sx); if ( sjtype != j_array ) { go_BYE(-1); }
  qtype_t sqtype = get_qtype(sx); if ( sqtype != TM ) { go_BYE(-1); }
  uint32_t sn    = get_arr_len(sx);  
  uint32_t ssz   = get_arr_size(sx);  

  const tm_t * const tmptr = get_arr_ptr(sx); 

  qtype_t dqtype = get_tm_qtype(fld); if ( dqtype <= 0 ) { go_BYE(-1); }

  status = make_num_array(NULL, sn, ssz, dqtype, ptr_dst); cBYE(status);
  void *dvals = get_arr_ptr(ptr_dst->data); 

  if ( strcmp(fld, "tm_year") == 0 ) { 
    for ( uint32_t i = 0; i < sn; i++ ) { 
      ((int16_t *)dvals)[i] = tmptr[i].tm_year; 
    }
  }
  else if ( strcmp(fld, "tm_mon") == 0 ) { 
    for ( uint32_t i = 0; i < sn; i++ ) { 
      ((int8_t *)dvals)[i] = tmptr[i].tm_mon; 
    }
  }
  else if ( strcmp(fld, "tm_mday") == 0 ) { 
    for ( uint32_t i = 0; i < sn; i++ ) { 
      ((int8_t *)dvals)[i] = tmptr[i].tm_mday; 
    }
  }
  else if ( strcmp(fld, "tm_hour") == 0 ) { 
    for ( uint32_t i = 0; i < sn; i++ ) { 
      ((int8_t *)dvals)[i] = tmptr[i].tm_hour; 
    }
  }
  /*
  else if ( strcmp(fld, "tm_min") == 0 ) { 
    for ( uint32_t i = 0; i < sn; i++ ) { 
      ((int8_t *)dvals)[i] = tmptr[i].tm_min; 
    }
  }
  else if ( strcmp(fld, "tm_sec") == 0 ) { 
    for ( uint32_t i = 0; i < sn; i++ ) { 
      ((int8_t *)dvals)[i] = tmptr[i].tm_sec; 
    }
  }
  */
  else if ( strcmp(fld, "tm_wday") == 0 ) { 
    for ( uint32_t i = 0; i < sn; i++ ) { 
      ((int8_t *)dvals)[i] = tmptr[i].tm_wday; 
    }
  }
  else if ( strcmp(fld, "tm_yday") == 0 ) { 
    for ( uint32_t i = 0; i < sn; i++ ) { 
      ((int16_t *)dvals)[i] = tmptr[i].tm_yday; 
    }
  }
  else {
    go_BYE(-1);
  }
BYE:
  return status;
}

int
x_time_band(
    const QDF_REC_TYPE *const ptr_in_qdf,
    uint32_t n_recent,
    QDF_REC_TYPE * restrict ptr_out_qdf
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_in_qdf, -1); 
  mcr_chk_null(ptr_out_qdf, -1); 
  char *x = ptr_in_qdf->data;
  jtype_t jtype = get_jtype(x); if ( jtype != j_array ) { go_BYE(-1); }
  qtype_t qtype = get_qtype(x); if ( qtype != TM ) { go_BYE(-1); }
  uint32_t n    = get_arr_len(x); 

  status = make_num_array(NULL, n, 0, I1, ptr_out_qdf); cBYE(status);
  char *y = ptr_out_qdf->data;

  const void * const inptr  = get_arr_ptr(x); 
  char *outptr = get_arr_ptr(y); 

  int8_t *I1ptr = (int8_t *)outptr;
  const tm_t * const tptr = (const tm_t * const )inptr;

  struct tm t1;
  memset(&t1, 0, sizeof(struct tm));
  t_assign(&t1, tptr+0);
  time_t maxt = tm2time(&t1);
  for ( uint32_t i = 1; i < n; i++ ) { 
    t_assign(&t1, tptr+i);
    time_t t = tm2time(&t1);
    if ( difftime(t, maxt) > 0 ) { 
      maxt = t;
    }
  }
  for ( uint32_t i = 0; i < n; i++ ) { 
    memset(&t1, 0, sizeof(struct tm)); 
    t_assign(&t1, tptr+i);
    time_t t = tm2time(&t1); 
    double d = difftime(maxt, t);
    if ( d > (n_recent * 86400) ) {
      I1ptr[i] = 0;
    }
    else {
      I1ptr[i] = 1;
    }
  }
BYE:
  return status;
}
/* From Mahdi:
 *
base_week <- max(as.Date(raw_input[[recency_random_effect$raw_name]]))
time_band <- ifelse(as.Date(raw_input$week_start_date) <= (base_week - 7 * recency_random_effect$n_recent), 0, 1)

time_band. It uses week_start_date and a number n_recent to create a factor variable with two levels: 0 (if week_start_date is more than n_recent away in the past from the maximum week_start_date in the data, and 1 otherwise).
*/


int
qdf_mktime(
    const QDF_REC_TYPE *const ptr_src,
    QDF_REC_TYPE * restrict ptr_dst
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_src, -1); 
  mcr_chk_null(ptr_dst, -1); 
  char *sx = ptr_src->data;
  jtype_t sjtype = get_jtype(sx); if ( sjtype != j_array ) { go_BYE(-1); }
  qtype_t sqtype = get_qtype(sx); if ( sqtype != TM ) { go_BYE(-1); }
  uint32_t sn    = get_arr_len(sx); 
  uint32_t ssz   = get_arr_size(sx);  

  const tm_t * const tmptr = get_arr_ptr(sx); 

  status = make_num_array(NULL, sn, ssz, I4, ptr_dst); cBYE(status);
  int32_t *ivals = get_arr_ptr(ptr_dst->data); 

  for ( uint32_t i = 0; i < sn; i++ ) { 
    struct tm t1;
    // TODO THINK t1.tm_zone = "GMT"; // TODO Experimental 
    memset(&t1, 0, sizeof(struct tm));
    t_assign(&t1, tmptr+i);
    // ivals[i] = mktime(&t1);
    time_t tempt = tm2time(&t1);
    if ( tempt > INT_MAX ) { 
      go_BYE(-1); } 
    ivals[i] = (int32_t)tempt; 
  }
BYE:
  return status;
}

