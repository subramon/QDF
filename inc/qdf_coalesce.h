extern int
coalesce(
    const QDF_REC_TYPE * const src1, // input numeric array F4
    const QDF_REC_TYPE * const nn_src1, // input numeric array I1
    const QDF_REC_TYPE * const src2, // input numeric array F4
    const QDF_REC_TYPE * const nn_src2, // input numeric array I1
    QDF_REC_TYPE * restrict dst, // output numeric array F4
    QDF_REC_TYPE * restrict nn_dst, // output numeric array I1
    int *ptr_num_nulls
   );
