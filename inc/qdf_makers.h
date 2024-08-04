#ifndef QDF_MAKERS_H
#define QDF_MAKERS_H
extern int
make_nil(
    void *dummy, // for function signature consistency
    QDF_REC_TYPE *ptr_qdf
    );
extern int
make_boolean(
    bool bval,
    QDF_REC_TYPE *ptr_qdf
    );
extern int
make_number(
    double num_val,
    QDF_REC_TYPE *ptr_qdf
    );
extern int
make_date(
    tm_t dateval,
    QDF_REC_TYPE *ptr_qdf
    );
extern int 
make_string(
    const char * const str_val,
    QDF_REC_TYPE *ptr_qdf
    );
extern int
make_SC_array(
    char ** svals, // [n_svals][..] 
    const char * concat_svals, // null separated svals
    uint32_t width, // space (including nullc) to be allocated per entry
    uint32_t arr_len,
    uint32_t arr_size,
    QDF_REC_TYPE *ptr_qdf
    );
extern int
make_bool_array(
    const bool *const vals,
    uint32_t arr_len,
    uint32_t arr_size,
    QDF_REC_TYPE *ptr_qdf
    );
extern int
make_num_array(
    const void *const vals,
    uint32_t n_vals, // how many elements in array
    uint32_t sz_vals, // how big array should be 
    qtype_t qtype,
    QDF_REC_TYPE * restrict ptr_qdf
    );
extern int
make_data_frame(
    char ** const cols,
    uint32_t n_cols,
    const uint32_t * const widths, // widths of columns 
    void **vals, // [n_cols][sz_rows]
    uint32_t n_rows,
    uint32_t sz_rows,
    const qtype_t *const qtypes, // [n_svals]
    QDF_REC_TYPE *ptr_qdf
    );
extern int
make_mixed_array_or_object(
    const QDF_REC_TYPE * const qdf_vals, // [n_qdfs]
    char ** const keys, // [n_qdfs]
    uint32_t n_qdfs,
    QDF_REC_TYPE *ptr_out_qdf
    );
extern int
append_mixed_array_or_object(
    QDF_REC_TYPE *ptr_dst_qdf,
    QDF_REC_TYPE *ptr_src_qdf
    );
extern int
make_empty_data_frame(
    char **const cols, // [n_cols]
    uint32_t n_cols,
    const char **const qtypes, // [n_cols]
    uint32_t sz_rows,
    QDF_REC_TYPE *ptr_qdf
    );
extern int
place_in_data_frame(
    QDF_REC_TYPE *ptr_dst, // dataframe to be modified in place
    const  char * const key, // name  of  column
    QDF_REC_TYPE *ptr_src // column to be placed into dst
    );
#endif //  QDF_MAKERS_H
