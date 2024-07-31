extern int
tm_extract(
    const QDF_REC_TYPE * const ptr_src,
    const char * const fld,
    QDF_REC_TYPE * restrict ptr_dst
    );
extern int
x_time_band(
    const QDF_REC_TYPE *const ptr_qdf,
    uint32_t n_recent,
    QDF_REC_TYPE * restrict ptr_out_qdf
    );
extern int
qdf_mktime(
    const QDF_REC_TYPE *const ptr_src,
    QDF_REC_TYPE * restrict ptr_dst
    );
