extern int
f_to_s_nn(
    const QDF_REC_TYPE *const ptr_qdf,
    const QDF_REC_TYPE *const ptr_nn_qdf,
    const char * const str_op,
    const SCLR_REC_TYPE *const ptr_in_sclr,
    SCLR_REC_TYPE * restrict ptr_out_sclr, // output 
    SCLR_REC_TYPE * restrict ptr_num_sclr // output 
    );
extern int 
sum_nn(
    const char * const X, 
    const int8_t * const nn_X, 
    uint32_t n, 
    qtype_t qtype, 
    SCLR_REC_TYPE * restrict ptr_num_sclr
    );
extern int 
min_nn(
    const char * const X, 
    const int8_t * const nn_X, 
    uint32_t n, 
    qtype_t qtype,
    SCLR_REC_TYPE * restrict ptr_out_sclr
    );
extern int 
max_nn(
    const char * const X, 
    const int8_t * const nn_X, 
    uint32_t n, 
    qtype_t qtype,
    SCLR_REC_TYPE * restrict ptr_out_sclr
    );
extern int 
sd_nn(
    const char  * const X, 
    const int8_t  * const nn_X, 
    uint32_t n, 
    qtype_t qtype, 
    const SCLR_REC_TYPE * const ptr_in_sclr,
    SCLR_REC_TYPE *ptr_out_sclr
    );
