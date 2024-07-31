#include "qdf_struct.h"
extern int 
free_qdfs(
    QDF_REC_TYPE **ptr_qdfs,  // [n] 
    const char * const mode,
    uint32_t n
    );
extern int
load_qdfs_from_files(
    char ** const infiles,  // [n_in] INPUT 
    uint32_t n_in,  // INPUT 
    bool is_writable, // INPUT 
    QDF_REC_TYPE **ptr_qdfs  // OUTPUT 
    );
extern int
bindmp(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const file_name
    );
extern int
binld(
    QDF_REC_TYPE *ptr_qdf,
    const char * const file_name,
    bool is_writable
    );
