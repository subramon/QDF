extern int
f_to_s(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const str_op,
    const SCLR_REC_TYPE *const ptr_in_sclr,
    SCLR_REC_TYPE * restrict ptr_out_sclr // output 
    );
extern int 
sum(
    const void * const X, 
    uint32_t n, 
    qtype_t qtype, 
    SCLR_REC_TYPE * restrict ptr_out_sclr
    );
extern int 
min(
    const void * const X, 
    uint32_t n, 
    qtype_t qtype,
    SCLR_REC_TYPE * restrict ptr_sclr
    );
extern int 
max(
    const void * const X, 
    uint32_t n, 
    qtype_t qtype, 
    SCLR_REC_TYPE *restrict ptr_sclr
    );
extern int 
sd(
    const void  * const X, 
    uint32_t n, 
    qtype_t qtype, 
    const SCLR_REC_TYPE * const ptr_in_sclr,
    SCLR_REC_TYPE *ptr_out_sclr
    );
