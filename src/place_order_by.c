#include "incs.h"
#include "configs.h"
#include "rs_hmap_struct.h"

#include "hive_struct.h"
#include "init_hive.h"
#include "free_hive.h"

#include "rs_hmap_int_types.h" // TODO P2 should we do this?
#include "rs_hmap_struct.h"
#include "rs_hmap_put.h"

#include "rs_mmap.h"
#include "get_time_usec.h"
#include "auxil.h"
#include "get_break_vals.h"
#include "line_to_cells.h"
#include "cells_to_vals.h"
#include "count_for_order_by_struct.h"
#include "aux_count_for_order_by.h"
#include "place_order_by.h"

static void 
sleep_a_bit(
    int sec,
    int nsec
    )
{
  struct timespec tmspec = { .tv_sec = sec, .tv_nsec = nsec } ;
  nanosleep(&tmspec, NULL);
}
void
place_order_by(
    const config_t * const C,
    count_for_order_by_struct_t *info,
    key_len_t *KO, // [nKO]
    int nKO,
    int *busy,
    char *outX,
    size_t noutX
    )
{
  int status = 0;
  uint64_t num_rows_read = 0;
  uint64_t num_bytes_read = 0;
  //---------------------------
  char *Y = NULL; uint32_t nY = 0, sz_Y = 64; // sz_Y set to default
  char *X = NULL; size_t nX = 0;
  if ( C == NULL ) { go_BYE(-1); }
  hive_struct_t H; memset(&H, 0, sizeof(hive_struct_t));
  // TODO P3 Make sure that we can order by with only 1 column
  //---------------------------
  info->t_start = get_time_usec();
  Y = malloc(sz_Y); return_if_malloc_failed(Y);
  if ( C->ddl_in.n_files <= 0 ) { go_BYE(-1); }
  if ( C->ddl_in.file_names == NULL ) { go_BYE(-1); }
  status = init_hive(C, &H); cBYE(status);
  char *max_outX = outX + noutX;

  uint64_t cum_filesz = 0; 
  int wid = info->wid; 
  int nW  = info->nW;
  uint64_t my_lb = wid   * info->per_filesz;
  uint64_t my_ub = my_lb + info->per_filesz;
  int my_file_idx = 0;
  if ( wid == 0 ) { my_lb = 0; }
  if ( wid == (nW-1) ) { my_ub = info->total_filesz; }
  for ( int f = 0; f < C->ddl_in.n_files; f++ ) { // iterate over files 
    char *file_name = C->ddl_in.file_names[f];
    status = rs_mmap(file_name, &X, &nX, 0); cBYE(status);
    // xidx, lnX are needed because we do not always process 0 to nX
    size_t xidx = 0, lnX = nX; 
    // decide whether to process (and how much to process) of this file 
    uint64_t this_lb = cum_filesz;
    uint64_t this_ub = this_lb + nX;
    bool skip_file = false; 
    if ( my_ub < this_lb ) { skip_file = true; } 
    if ( my_lb > this_ub ) { skip_file = true; }  // TODO > or >= ?
    if ( skip_file ) { 
      cum_filesz += nX; mcr_rs_munmap(X, nX); continue;
    }
    if ( ( my_lb < this_lb ) && ( my_ub >= this_ub ) ) {
      // process whole file 
    }
    else {
      if ( ( my_lb <= this_lb ) && ( my_ub < this_ub ) ) {
        lnX = my_ub - this_lb;
      }
      if ( ( my_lb > this_lb ) && ( my_ub >= this_ub ) ) {
        xidx +=  my_lb - this_lb;
      }
    }
    // adjust xidx/lnX so that we start on a line boundary
    // both need to be adjusted down
    for ( ; ; ) { 
      if ( xidx == 0 ) { break; }
      if ( X[xidx-1] == C->ddl_in.rec_sep ) { break; }
      xidx--;
    }
    for ( ; ; ) { 
      if ( lnX == 0 ) { break; }
      if ( X[lnX-1] == C->ddl_in.rec_sep ) { break; }
      lnX--;
    }
    num_bytes_read += (lnX - xidx);
    //------------------------------------------------
    __builtin_prefetch (X+xidx, 0, 0);  // for next line 
    /*
    printf("%d processing file %d:%d:%s, [%lu, %lu]\n", 
        wid, f, my_file_idx, file_name, xidx, lnX);
	*/
    my_file_idx++;
    int lno = 0; // for debugging 
    for ( ; xidx < lnX ; lno++ ) { // NOTE: Not nX 
      size_t yidx;
      memset(Y, 0, nY); 
      // gather data into a line 
      for ( yidx = 0; xidx < nX; ) {
        // re-alloc if needed
        if ( yidx >= sz_Y ) { 
          sz_Y *= 2; Y = realloc(Y, sz_Y);
        }
        //---------------
        char c = X[xidx];
        Y[yidx++] = X[xidx++];
        if ( c == C->ddl_in.rec_sep ) { 
          break;
        }
      }
      nY = yidx;
      // We now have a line of length nY
      // break down the line into individual cells
      status = line_to_cells(Y, nY, C->in_meta.n_cols, 
          C->ddl_in.fld_sep, C->ddl_in.rec_sep, H.cells, H.cellsz); 
      cBYE(status);
      status = get_break_vals(H.cells, C->in_meta.n_cols, 
          C->ddl_in.break_cols, C->ddl_in.n_break_cols, H.break_vals); 
      cBYE(status);
      //-----------
      // make a composite key 
      rs_hmap_key_t key =  H.break_vals[0];
      for ( int i = 1; i < C->ddl_in.n_break_cols; i++ ) { 
        key = ( key << 32 ) | H.break_vals[i];
      }
      // START: Place the key in the big file 
      // Find the compkey in K
      int kidx;
      status = key_bin_search(KO, nKO, key, &kidx); 
      if ( kidx < 0 ) { go_BYE(-1); }
      // Lock the offset for kidx 
      for ( ; ; ) {
	int expected =  0; 
	int desired   = 1; 
	bool rslt = __atomic_compare_exchange(
	busy+kidx, &expected, &desired, false, 
	__ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
	if ( rslt ) { break; }
	sleep_a_bit(0, 100000);
	info->num_sleeps++; 
      }
      // Get the offset 
      size_t this_offset = KO[kidx].offset;
      // Update the offset 
      KO[kidx].offset += nY;
      // UnLock the offset for kidx 
      int itmp = 0;
      __atomic_store(busy+kidx, &itmp,  0);
      // Place the line 
      if ( this_offset+nY > info->total_filesz ) { go_BYE(-1); }
      if ( outX + this_offset + nY > max_outX ) { go_BYE(-1); }
      memcpy(outX+this_offset, Y, nY); 
      // STOP : Place the key in the big file 
      num_rows_read++;
    }
    cum_filesz += nX; mcr_rs_munmap(X, nX);
  }
  info->num_rows_read  = num_rows_read;
  info->num_bytes_read = num_bytes_read;
  info->t_stop = get_time_usec();
BYE:
  info->status = status; 
  mcr_rs_munmap(X, nX);
  free_if_non_null(Y);
  free_hive(&H, C);
}
