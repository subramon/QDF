extern int
qdf_copy_array(
    QDF_REC_TYPE *dst,
    const QDF_REC_TYPE * const src
   );
extern int
coerce(
    void *dst, 
    qtype_t dst_qtype,
    const void * const src,
    qtype_t src_qtype,
    uint32_t sn
    );
