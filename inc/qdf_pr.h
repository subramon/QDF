extern int
pr_df_as_csv(
    const QDF_REC_TYPE * const qdf,
    const char **  const keys,
    uint32_t n_keys,
    const char * const file_name
    );
extern int
pr_json(
    const QDF_REC_TYPE * const ptr_qdf,
    QDF_REC_TYPE * restrict ptr_out_str, // where output string is created
    uint32_t *ptr_buflen, // how much of allocated space has been used 
    FILE *fp // note that either fp or out_str is provided, not both
    );
extern int
pr_1(
    const void * const valptr, 
    qtype_t qtype, 
    uint32_t width,
    uint32_t idx,
    FILE *fp
    );
extern int
x_pr_array(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const file_name
    );
extern int
pr_csv(
    const QDF_REC_TYPE * const src,
    const char * const file_name
    );
extern int 
qdf_to_json_file(
    QDF_REC_TYPE *ptr_qdf, 
    const char * const template,
    char **ptr_json_file
    );
