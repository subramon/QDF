extern uint32_t
x_get_obj_len(
    const QDF_REC_TYPE * const ptr_qdf
    );
extern uint32_t
x_get_obj_arr_size(
    const QDF_REC_TYPE * const ptr_qdf
    );
extern uint32_t
x_get_obj_arr_len(
    const QDF_REC_TYPE * const ptr_qdf
    );
extern uint32_t
x_get_arr_width(
    const QDF_REC_TYPE *const ptr_qdf
    );
extern uint32_t
x_get_arr_len(
    const QDF_REC_TYPE *const ptr_qdf
    );
extern uint32_t
x_get_arr_size(
    const QDF_REC_TYPE *const ptr_qdf
    );
extern jtype_t 
x_get_jtype( // x_* functions are to be invoked from Lua 
    const QDF_REC_TYPE *const ptr_qdf
    );
extern uint32_t
x_get_poff( // x_* functions are to be invoked from Lua 
    const QDF_REC_TYPE *const ptr_qdf
    );
extern qtype_t
x_get_qtype( // x_* functions are to be invoked from Lua 
    const QDF_REC_TYPE *const ptr_qdf
    );
extern uint32_t
x_get_qdf_size(
    const QDF_REC_TYPE * const ptr_qdf
    );
extern double
x_get_num_val( // x_* functions are to be invoked from Lua 
    QDF_REC_TYPE *ptr_qdf
    );
extern int
x_get_date_val( // x_* functions are to be invoked from Lua 
    const QDF_REC_TYPE *const ptr_qdf,
    tm_t *ptr_tm
    );
extern char * 
x_get_str_val( // x_* functions are to be invoked from Lua 
    QDF_REC_TYPE *ptr_qdf
    );
extern bool
x_get_bool_val( // x_* functions are to be invoked from Lua 
    QDF_REC_TYPE *ptr_qdf
    );
extern uint32_t
x_get_str_len(
    QDF_REC_TYPE *ptr_qdf
    );
extern int
x_get_arr_ptr(
    QDF_REC_TYPE *ptr_in,
    QDF_REC_TYPE *ptr_out
    );
extern char *
x_get_offsets(
    QDF_REC_TYPE *ptr_qdf
    );
extern bool
x_get_is_df(
    const QDF_REC_TYPE * const ptr_qdf
    );
extern int
x_set_obj_arr_size(
    const QDF_REC_TYPE * const ptr_qdf,
    uint32_t arr_size
    );
extern int
x_set_obj_arr_len(
    const QDF_REC_TYPE * const ptr_qdf,
    uint32_t arr_len
    );
extern int
x_set_arr_len(
    const QDF_REC_TYPE * const ptr_qdf,
    uint32_t arr_len
    );
extern int
x_set_is_df(
    const QDF_REC_TYPE * const ptr_qdf,
    bool is_df
    );
extern int
x_set_qtype(
    const QDF_REC_TYPE * const ptr_qdf,
    qtype_t qtype
    );
extern int
x_set_num_val(
    const QDF_REC_TYPE * const ptr_qdf,
    double num_val
    );
