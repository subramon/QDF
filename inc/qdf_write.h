extern int
qdf_write(
    const QDF_REC_TYPE * const src, // INPUT 
    uint32_t offset, // INPUT
    bool truncate, // INPUT
    QDF_REC_TYPE * restrict dst // INPUT and OUTPUT 
   );
