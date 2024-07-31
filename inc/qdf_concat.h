extern int
qdf_concat(
    const QDF_REC_TYPE * const in_qdfs,  // [n_qdfs]  INPUT 
    uint32_t n_qdfs, // INPUT 
    const char ** const keys, // [n_keys] INPUT 
    uint32_t n_keys, // INPUT
    QDF_REC_TYPE *ptr_out_qdf
    );
