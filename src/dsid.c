#include "incs.h"
#include "qtypes.h"
#include "dsid_struct.h"
#include "dsid.h"
#include "configs.h"

int 
convert_str_break_val(
    const char * const buf, 
    qtype_t qtype,
    dsid_t *ptr_dsid,
    uint32_t i
    )
{
  int status = 0; 
  ptr_dsid->is_str = 0;  // default assumption all IDs are uint32_t
  if ( i >= MAX_NUM_BREAK_VALS ) { go_BYE(-1); }
  // cannot be null string
  if ( ( buf == NULL ) || ( *buf == '\0' ) ) { go_BYE(-1); } 
  memset(&(ptr_dsid->sid[i]), 0, MAX_LEN_DSID+1);
  switch ( qtype ) { 
    case I1 : 
    case I2 : 
    case I4 : 
    case I8 : 
      {
        char *endptr = NULL; 
        long long int tmpI8 = strtoll(buf,  &endptr, 10); 
        if ( *endptr != '\0' ) { go_BYE(-1); }
        ptr_dsid->uid[i] = (uint32_t)tmpI8;
      }
      break;
    case SC : // this was invented for item_cluster_c
      {
        if ( strlen(buf) > MAX_LEN_DSID ) { go_BYE(-1); }
        strcpy(ptr_dsid->sid[i], buf);
        // remember to set is_str to indicate ith entry is string 
        ptr_dsid->is_str = ptr_dsid->is_str | ((uint32_t)1 << i);
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
int
get_break_vals(
    char ** const cells,  // input  [n_cols][..]
    uint32_t n_cols, // input 
    const qtype_t * const qtypes, // input
    const int * const break_cols, // input  [n_break_cols]
    uint32_t n_break_cols, // input 
    dsid_t *ptr_dsid
    )
{
  int status = 0;

  if ( cells == NULL ) { go_BYE(-1); }
  if ( qtypes == NULL ) { go_BYE(-1); }
  if ( break_cols == NULL ) { go_BYE(-1); }
  if ( n_cols <= 0 ) { go_BYE(-1); } 
  if ( n_break_cols <= 0 ) { go_BYE(-1); }
#ifdef DEBUG
  // break_cols should be unique and should index a valid column
  for ( uint32_t i = 0; i < n_break_cols; i++ ) { 
    if (( break_cols[i] < 0)||(break_cols[i] >= (int)n_cols )){go_BYE(-1);}
    for ( uint32_t j = 0; j < n_break_cols; j++ ) { 
      if ( i != j ) { 
        if ( break_cols[i] == break_cols[j] ) { go_BYE(-1); }
      }
    }
  }
#endif
  memset(ptr_dsid, 0, sizeof(dsid_t));
  for ( uint32_t i = 0; i < n_break_cols; i++ ) { 
    if ( break_cols[i] < 0 ) { go_BYE(-1); } 
    uint32_t colidx = (uint32_t)break_cols[i];
    const char *buf = cells[colidx];
    status = convert_str_break_val(buf, qtypes[colidx], ptr_dsid, i);
    cBYE(status);
  }
BYE:
  return status;
}
/*
int
pr_dsid(
    dsid_t dsid,
    const config_t * const C,
    const char * const eq_str,
    const char * const sep_str,
    FILE *fp,
    char **ptr_out_str
    )
{
  int status = 0;
  *ptr_out_str = NULL; 
  if ( fp != NULL ) { // TODO P Return out_str not implemenred
    go_BYE(-1);
  }
  for ( uint32_t i = 0; i < C->ddl_in.n_break_cols; i++ ) { 
    if ( i > 0 ) { fprintf(fp, "%s", sep_str); }
    fprintf(fp, "%s%s", C->ddl_in.str_break_cols[i], eq_str);
    if ( ( ( dsid.is_str >> i ) & 0x1) == 1 ) {
      fprintf(fp, "\"%s\"", dsid.sid[i]);
    }
    else {
      fprintf(fp, "%u", dsid.uid[i]);
    }
  }
BYE:
  return status;
}

*/
int
dsid_pr_1(
  dsid_t dsid,
  FILE *fp
  )
{
  int status = 0;
  fprintf(fp, "(");
  for ( int i = 0; i < MAX_NUM_BREAK_VALS; i++ ) {
    if ( i > 0 ) { fprintf(fp, ","); }
    if ( ( ( dsid.is_str >> i ) & 0x1) == 1 ) {
      fprintf(fp, "\"%s\"", dsid.sid[i]);
    }
    else {
      fprintf(fp, "%u", dsid.uid[i]);
    }
  }
  fprintf(fp, ")");
  return status;
}
