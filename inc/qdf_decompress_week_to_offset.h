extern int
decompress_week_to_offset(
    const QDF_REC_TYPE * const src1,
    const QDF_REC_TYPE * const src2,
    const date_hol_t * const H, // [nH]
    uint32_t nH, 
    QDF_REC_TYPE * restrict dst
    );
