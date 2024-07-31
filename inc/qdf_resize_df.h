extern int 
qdf_resize_df(
    QDF_REC_TYPE *ptr_old,
    char ** in_keys, // [in_n_keys]
    char ** in_str_qtypes, // [in_n_keys]
    uint32_t in_n_keys,
    uint32_t new_sz,
    QDF_REC_TYPE *ptr_new
    );
