#ifndef Q_MACROS_H
#define Q_MACROS_H
#define WHEREAMI { fprintf(stderr, "Line %3d of File %s \n", __LINE__, __FILE__);  }
#define go_BYE(x) { WHEREAMI; status = x ; goto BYE; }
#define omp_BYE(x) { WHEREAMI; status = x ; continue; }
#define cBYE(x) { if ( (x) != 0 ) { go_BYE((x)) } }
#define c_omp_BYE(x) { if ( (x) < 0 ) { omp_BYE((x)) } }
#define fclose_if_non_null(x) { if ( (x) != NULL ) { fclose((x)); (x) = NULL; } } 
#define free_if_non_null(x) { if ( (x) != NULL ) { free((x)); (x) = NULL; } }
#define return_if_fopen_failed(fp, file_name, access_mode) { if ( fp == NULL ) { fprintf(stderr, "Unable to open file %s for %s \n", file_name, access_mode); go_BYE(-1); } }
#define return_if_malloc_failed(x) { if ( x == NULL ) { fprintf(stderr, "Unable to allocate memory\n"); go_BYE(-1); } }

#define rs_munmap(X, nX)  { if ( ( X != NULL ) && ( nX != 0 ) ) { munmap(X, nX); } }
#define mcr_max(X, Y)  ((X) > (Y) ? (X) : (Y))
#define sqr(X)  ((X) * (X))

#define mcr_chk_non_null(x, y) {  \
  if ( x == NULL ) { WHEREAMI; return y; } \
  if ( x->data == NULL ) { WHEREAMI; return y; } \
  if ( x->size == 0 ) { WHEREAMI; return y; } \
}
#define mcr_chk_null(x, y) {  \
  if ( x == NULL ) { WHEREAMI; return y; } \
  if ( x->data != NULL ) { WHEREAMI; return  y; } \
  if ( x->name != NULL ) { WHEREAMI; return y; } \
  if ( x->size != 0 ) { WHEREAMI; return y; } \
  if ( ( x->is_foreign != 0 )  && ( x->is_foreign != 1 ) ) { WHEREAMI; return y; } \
  if ( ( x->is_mmap != 0 )  && ( x->is_mmap != 1 ) ) { WHEREAMI; return y; } \
  if ( ( x->is_read_only != 0 )  && ( x->is_read_only != 1 ) ) { WHEREAMI; return y; } \
}

#endif // Q_MACROS_H
