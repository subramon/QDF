extern int
decompress(
    const QDF_REC_TYPE * const src1,
    const QDF_REC_TYPE * const src2,
    compress_mode_t compress_mode,
    QDF_REC_TYPE *dst
    );
extern int
decompress_distinct_vals(
    const QDF_REC_TYPE * const src1,
    const QDF_REC_TYPE * const src2,
    qtype_t qtype,
    QDF_REC_TYPE *dst
    );
extern int
decompress_F4_to_F2(
    const QDF_REC_TYPE * const src,
    QDF_REC_TYPE *dst
    );
