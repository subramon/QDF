extern int
qdf_incr_on_by(
    const QDF_REC_TYPE *const ptr_src, // input
    const SCLR_REC_TYPE * const ptr_on,
    const SCLR_REC_TYPE * const ptr_incr,
    bool reset,
    QDF_REC_TYPE * restrict ptr_dst // output 
   );
