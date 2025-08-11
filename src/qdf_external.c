#include "incs.h"
#include "qdf_struct.h"
#include "rs_mmap.h"
#include "qdf_external.h"
#include "qdf_helpers.h"
#ifdef DEBUG
#include "qdf_checkers.h"
#endif
int 
free_qdfs(
    QDF_REC_TYPE **ptr_qdfs,  // [n] 
    const char * const mode,
    uint32_t n
    ) 
{
  int status = 0;
  QDF_REC_TYPE *qdfs = *ptr_qdfs;
  if ( qdfs == NULL ) { go_BYE(-1); }
  if ( n    == 0    ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n; i++ ) { 
    char *X = qdfs[i].data;
    size_t nX = qdfs[i].size;
    if ( strcmp(mode, "munmap") == 0 ) { 
      mcr_rs_munmap(X, nX);
    }
    else if ( strcmp(mode, "free") == 0 ) { 
      if ( qdfs[i].is_foreign == false ) { 
        free_if_non_null(X); nX = 0;
      }
    }
    else {
      go_BYE(-1); 
    }
    memset(qdfs+i, 0, sizeof(QDF_REC_TYPE));
  }
  free_if_non_null(qdfs);
  *ptr_qdfs = NULL;
BYE:
  return status;
}
int
load_qdfs_from_files(
    char ** const infiles,  // [n_in] INPUT 
    uint32_t n_in,  // INPUT 
    bool is_writable,
    QDF_REC_TYPE **ptr_qdfs  // OUTPUT 
    )
{
  int status = 0;
  QDF_REC_TYPE *qdfs = NULL;
  //--- Allocatte space for in_qdfs 
  qdfs = malloc(n_in * sizeof(QDF_REC_TYPE));
  memset(qdfs, 0,  n_in * sizeof(QDF_REC_TYPE));
  // load in_qdfs from in_files
  for ( uint32_t i = 0; i < n_in; i++ ) { 
    status = binld(qdfs+i, infiles[i], is_writable);  cBYE(status);
#ifdef DEBUG
    status = chk_qdf(&(qdfs[i])); cBYE(status);
#endif
  }
  *ptr_qdfs = qdfs;
BYE:
  return status;
}

int
bindmp(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const file_name
    )
{
  int status = 0;
  FILE *fp = NULL;

  fp = fopen(file_name, "wb");
  return_if_fopen_failed(fp,  file_name, "wb");
  if ( ( ptr_qdf->data == NULL ) || ( ptr_qdf->size == 0 ) ) {
    go_BYE(-1);
  }
  size_t nw = fwrite(ptr_qdf->data, 1, ptr_qdf->size, fp);
  if ( nw != ptr_qdf->size ) { go_BYE(-1); }
BYE:
  fclose_if_non_null(fp);
  return status;
}
  
int
binld(
    QDF_REC_TYPE *ptr_qdf,
    const char * const file_name,
    bool is_writable
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;

#ifdef DEBUG
  if ( file_name == NULL ) { go_BYE(-1); }
  if ( !isfile(file_name) ) { go_BYE(-1); }
  // ONLY FOR DEBUGGING printf("XXX mmap %s \n", file_name); 
  // ONLY FOR DEBUGGING ! ptr_qdf->name = strdup(file_name); 
#endif
  status = rs_mmap(file_name, &X, &nX, is_writable); cBYE(status);
  ptr_qdf->data = X;
  if ( nX == 0 ) { go_BYE(-1); }
  if ( nX > UINT_MAX ) { go_BYE(-1); }
  ptr_qdf->size = (uint32_t)nX;
  ptr_qdf->is_mmap = true;
  ptr_qdf->is_foreign = false;
  if ( !is_writable ) { 
    ptr_qdf->is_read_only = true;
  }
#ifdef DEBUG
  status = chk_qdf(ptr_qdf); cBYE(status);
#endif
BYE:
  return status;
}
