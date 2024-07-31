extern int
squeeze_where(
    QDF_REC_TYPE * restrict dst, // object consisting of numeric arrays
    // or a numeric array 
    const QDF_REC_TYPE *const where, // numeric I1 array (1=> good, 0 => null)
    uint32_t *ptr_num_good
   );
