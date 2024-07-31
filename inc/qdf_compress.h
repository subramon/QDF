extern int
compress(
    const QDF_REC_TYPE *const src,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2,
    compress_mode_t *ptr_compress_mode
    );
extern int
compress_distinct_vals(
    const QDF_REC_TYPE *const src,
    qtype_t src_qtype,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_distinct_vals_F4(
    const QDF_REC_TYPE *const src,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_distinct_vals_I2(
    const QDF_REC_TYPE *const src,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_distinct_vals_I4(
    const QDF_REC_TYPE *const src,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_F4_to_F2(
    const QDF_REC_TYPE *const src,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_none(
    const QDF_REC_TYPE * const src,
    QDF_REC_TYPE * dst1,
    QDF_REC_TYPE * dst2
    );
extern int
compress_week_to_offset(
    const QDF_REC_TYPE *const src,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
