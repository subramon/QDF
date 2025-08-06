#include "incs.h"
#include "qdf_mem.h"

QDF_REC_TYPE *
qdf_cmem_make(
    uint32_t size,
    const char * const name
    )
{
  int status = 0;
  static int ctr = 0; 
  void *y = NULL;
  QDF_REC_TYPE *x = NULL; 

  status = posix_memalign(&y, 16, sizeof(QDF_REC_TYPE));
  if ( status != 0 ) { WHEREAMI; return NULL; } 
  x = (QDF_REC_TYPE *)y;
  memset(x, 0, sizeof(QDF_REC_TYPE));
  x->size = size;
  if ( size > 0 ) { 
    x->data = malloc(size);
    memset(x->data, 0, size);
    if ( x->data == NULL ) { WHEREAMI; return NULL; }
  }
  if ( name != NULL ) {
    x->name = strdup(name);
    if ( x->name == NULL ) { WHEREAMI; return NULL; }
  }
  x->id = ++ctr;
  // printf("Making %d of size %u \n", x->id, size); 
  return x;
}

char * 
qdf_cmem_get_name(
  QDF_REC_TYPE *x
)

{
  if ( x == NULL ) { return NULL; }
  return x->name;
}

int 
qdf_cmem_set_name(
  QDF_REC_TYPE *x,
  const char * const name
)

{
  int status = 0; 
  if ( x == NULL ) { go_BYE(-1); } 
  if ( x->is_read_only ) { go_BYE(-1); } 
  free_if_non_null(x->name);
  if ( name != NULL ) { 
    x->name = strdup(name);
  }
BYE:
  return status;
}

int 
qdf_cmem_free(
  QDF_REC_TYPE *x
)

{
  if ( x == NULL ) { WHEREAMI; /* unspected */ return -1; }
  // printf("Freeing %d of size %u\n", x->id, x->size); 
  // TODO P1 Following line looks suspicious????
  if ( x->is_foreign ) { free_if_non_null(x); return 0; } 

  //---------------------------------------------------

  if ( x->data == NULL ) { 
    if ( x->is_err ) { // silent failure
      memset(x, 0, sizeof(QDF_REC_TYPE)); return 0; 
    }
    else {
      WHEREAMI; return -1; 
    }
  }
  //---------------------------------------------------
  if ( ( ( x->data == NULL ) && ( x->size != 0 ) )  ||
       ( ( x->data != NULL ) && ( x->size == 0 ) ) ) {
    WHEREAMI; return -1; 
  }
  //---------------------------------------------------
  if ( x->is_mmap ) { 
    munmap(x->data, x->size); 
  }
  else {
    free_if_non_null(x->data);
  }
  free_if_non_null(x->name);
  memset(x, 0, sizeof(QDF_REC_TYPE)); 
  free(x); // IMPORTANT;
  return 0;
}

void
qdf_cmem_nop(
  QDF_REC_TYPE *x
)
{
  if ( ( x == NULL ) && ( x->name != NULL ) ) { 
    printf("NOP on [%s] \n", x->name);
  }
}
QDF_REC_TYPE *
qdf_cmem_clone(
    QDF_REC_TYPE *inx
    )
{
  int status = 0;
  void *x = NULL; 
  QDF_REC_TYPE *outx = NULL;
  if ( inx == NULL ) { WHEREAMI; return NULL; }
  status = posix_memalign(&x, 16,  sizeof(QDF_REC_TYPE)); 
  if ( ( status != 0 ) || ( x == NULL ) ) { WHEREAMI; return NULL; }
  outx = (QDF_REC_TYPE *)x;
  memset(outx, 0, sizeof(QDF_REC_TYPE));
  uint32_t insize = inx->size;
  if ( insize == 0 ) { return outx; } // NOTE: possible early return 
  // printf("Cloning of size %lu  \n", insize); 

  outx->size = insize;
  status = posix_memalign(&(outx->data), 16, insize);
  if ( status != 0 ) { WHEREAMI; return NULL; }
  memcpy(outx->data, inx->data, insize);

  if ( inx->name != NULL ) {
    outx->name = strdup(inx->name);
    if ( outx->name == NULL ) { WHEREAMI; return NULL; }
  }
  outx->is_foreign = false; 
  outx->is_mmap = false; 
  outx->is_read_only = false; 
  return outx;
}
#define NEEDED
#ifdef NEEDED
// make a "soft" copy i.e., we should not delete stuff created by a dupe
// hence we set is_foreign := true 
QDF_REC_TYPE *
qdf_cmem_dupe(
    QDF_REC_TYPE *y
    )
{
  int status = 0;
  if ( y == NULL ) { WHEREAMI; return NULL; }
  void *z = NULL;
  QDF_REC_TYPE *x = NULL;
  status = posix_memalign(&z, 16, sizeof(QDF_REC_TYPE));
  x = (QDF_REC_TYPE *)z;
  // printf("size = %u \n", y->size); 
  if ( status != 0 ) { WHEREAMI; return NULL; } 
  memcpy(x, y, sizeof(QDF_REC_TYPE));
  x->is_foreign = true; // IMPORTANT 
  return x;
}
#endif //  NEEDED
