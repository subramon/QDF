extern int
qdf_named_vec(
    char * const key_blob, // [n_blob] 
    uint32_t n_blob,
    const double * const dvals, // [n] 
    uint32_t n_vals,
    QDF_REC_TYPE * restrict ptr_out_qdf
    );
