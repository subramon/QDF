#include "incs.h"
#include "configs.h"
#include "qdf_struct.h"
#include "dsid_struct.h"
#include "write_to_disk.h"
#include "qdf_external.h"
#include "qdf_checkers.h"
#include "qdf_helpers.h"
#include "qdf_clone.h"
#include "load_qdf.h"
int 
load_qdf(
        char ** const str_break_cols, // from C->ddl_in
        uint32_t n_break_cols, // from C->ddl_in
        const char * const dir,
        const char * const prefix,
        dsid_t dsid,
        bool is_writable,
        QDF_REC_TYPE *ptr_dst
        )
{
  int status = 0;
  mcr_chk_null(ptr_dst, -1);
  // create name of file where the data is stored for this key
  char file_name[QDF_MAX_LEN_FILE_NAME+1];
  memset(file_name, 0, QDF_MAX_LEN_FILE_NAME+1);
  status = make_save_file_name(dir, prefix, NULL,
      str_break_cols, dsid, n_break_cols, file_name);
  cBYE(status);
  //--------------------------------------------------------
  if ( !is_writable ) {
    status = binld(ptr_dst, file_name, false); cBYE(status);
  }
  else {
    QDF_REC_TYPE tmp_qdf; memset(&tmp_qdf, 0, sizeof(QDF_REC_TYPE));
    status = binld(&tmp_qdf, file_name, false); cBYE(status);
    status = qdf_clone(&tmp_qdf, ptr_dst); cBYE(status);
    free_qdf(&tmp_qdf);
  }
#ifdef DEBUG
  status = chk_qdf(ptr_dst); cBYE(status);
#endif
BYE:
  return status;
}
