extern int
ifxthenyelsez(
    const QDF_REC_TYPE * const x, // "x" input numeric array 
    const QDF_REC_TYPE * const y, // "y" input numeric array 
    const QDF_REC_TYPE * const z, // "z" input numeric array 
    QDF_REC_TYPE *dst // output 
   );
extern int
ifxthenyelsez_sclr(
    const QDF_REC_TYPE * const x, // "x" input numeric array 
    const QDF_REC_TYPE * const y, // "y" input numeric array 
    const SCLR_REC_TYPE * const z, // "z" input scalar
    QDF_REC_TYPE *dst // output 
   );
