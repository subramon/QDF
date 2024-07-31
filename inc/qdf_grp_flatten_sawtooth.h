extern int
qdf_grp_flatten_sawtooth(
    const QDF_REC_TYPE *const ptr_src, // input
    const QDF_REC_TYPE *const ptr_n_grp, // input
    QDF_REC_TYPE * restrict ptr_dst // output 
   );
extern int
grp_flatten_sawtooth_I1(
    const int8_t *const s, // [sn] 
    uint32_t sn,
    const int32_t * const n_grp, // [gn]
    uint32_t gn,
    int8_t *d // [sn]
    );
