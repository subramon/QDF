#ifndef __QDF_REC_TYPE
#define __QDF_REC_TYPE
typedef struct _qdf_rec_type {
  void *data;
  uint32_t size; // must be a multiple of 8 
  bool is_mmap; // true => we have mmapped data not malloc'd it 
  bool is_foreign; // true => do not free() or munmap() 
  bool is_read_only; // true => don't modify
} QDF_REC_TYPE;
#endif // __QDF_REC_TYPE
