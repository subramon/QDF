extern int
qdf_grp_sawtooth(
    const QDF_REC_TYPE *const ptr_src, // input
    const QDF_REC_TYPE *const ptr_n_grp, // input
    const char * const str_dqtype,
    QDF_REC_TYPE * restrict ptr_dst // output 
   );
extern int
grp_sawtooth_I1_I1(
    const int8_t *const src, // [sn] 
    uint32_t sn,
    const int32_t * const n_grp, // [gn]
    uint32_t gn,
    int8_t *dst // [sn]
    );
