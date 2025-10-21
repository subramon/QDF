return [[
// IMPORTANT
// TODO P2 Because of some legacy stuff, this file is duplicated in 
// Q/UTILS/inc/. If you make a change here, copy the file over there
//START_FOR_CDEF
typedef uint16_t bfloat16; 
typedef enum { 
  Q0, // mixed  must be first one 

  B1, // boolean as a bit
  BL, // boolean as a bool

  I1,
  I2,
  I4,
  I8,
  I16,

  F2,
  F4,
  F8,

  UI1,
  UI2,
  UI4,
  UI8,
  UI16,

  SC,  // constant length strings
  SV,  // variable length strings
  TM,  // struct tm 
  TM1, // tm_t

  QF // must be last one 
} qtype_t;
typedef struct _tm_t {
  int16_t tm_year;	/* Year	- 1900. */
  int8_t tm_mon;	/* Month.	[0-11] */
  int8_t tm_mday;	/* Day.		[1-31] */
  int8_t tm_hour;	/* Hours.	[0-23] */
  // int8_t tm_min;	/* Minutes.	[0-59] */
  // int8_t tm_sec;	/* Seconds.	[0-60] (1 leap second) */
  int8_t tm_wday;	/* Day of week.	[0-6] */
  int16_t tm_yday;	/* Days in year.[0-365]	*/

  /* Not being used 
  int tm_isdst;			// DST.		[-1/0/1]
  long int tm_gmtoff;		// Seconds east of UTC.  
  const char *tm_zone;		// Timezone abbreviation.  
  long int __tm_gmtoff;		// Seconds east of UTC.  
  const char *__tm_zone;	// Timezone abbreviation.  
  */
} tm_t;
//STOP_FOR_CDEF
extern qtype_t
get_tm_qtype(
    const char * const fld
    );
extern int
t_assign(
    struct tm *dst, 
    tm_t *src
    );
extern char *
get_format(
    const char * const str_qtype
    );
extern uint32_t
get_width_qtype(
    const char * const str_qtype
    );
extern uint32_t
get_width_c_qtype(
      qtype_t qtype
    );
extern qtype_t
get_c_qtype(
    const char *const str_qtype
    );
extern const char *
get_str_qtype(
    qtype_t qtype
    );
extern const char *
str_qtype_to_str_ctype(
    const char * const str_qtype
    );
extern bool
is_qtype(
    const char *const str_qtype
    );
extern const char *
str_qtype_to_str_ispctype(
    const char * const str_qtype
    );
extern bfloat16
F4_to_F2(
    float x
    );
extern float
F2_to_F4(
    bfloat16 x
    );
extern char *
qtypes_as_lua_tbl(
    void
    );



typedef struct _cmem_rec_type {
  void *data;
  int64_t size;
  qtype_t qtype;
  char cell_name[15 +1];
  bool is_foreign;
  bool is_stealable;
} CMEM_REC_TYPE;


typedef struct _sclr_rec_type {
  char name[15 +1];
  union {
    bool bl;

    int8_t i1;
    int16_t i2;
    int32_t i4;
    int64_t i8;

    uint8_t ui1;
    uint16_t ui2;
    uint32_t ui4;
    uint64_t ui8;

    bfloat16 f2;
    float f4;
    double f8;
    char *str;
    tm_t tm1;
  } val;
  qtype_t qtype;

} SCLR_REC_TYPE;
// TODO: Keep in sync with lua/jtypes.lua
//START_FOR_CDEF
typedef enum { 
  j_undef, 
  j_nil, 
  j_string,
  j_number, 
  j_date, 
  j_array, 
  j_object,
  j_bool, 
} jtype_t;

typedef enum _compress_mode_t { 
  c_undef,
  c_none,
  c_distinct_vals,
  c_F4_to_F2,
  c_week_to_offset
} compress_mode_t;

typedef struct {
  union Data {
    const char *strval;
    double dval;
    bool bval;
  } data;
  int str_len;
  jtype_t jtype;
} PRIMITIVE_VAL_TYPE;

typedef struct _qdf_hdr_t { 
  uint8_t jtype;
  uint8_t qtype;
  uint8_t unused1;
  uint8_t unused2;
  uint32_t poff;
} qdf_hdr_t;

typedef struct _qdf_bool_hdr_t { 
  uint8_t jtype;
  uint8_t qtype;
  bool    bool_val; 
  uint8_t unused1;
  uint32_t poff;
} qdf_bool_hdr_t;

typedef struct _qdf_number_hdr_t { 
  uint8_t jtype;
  uint8_t qtype;
  uint8_t unused1;
  uint8_t unused2;
  uint32_t poff;
  double num_val;
} qdf_number_hdr_t;

typedef struct _qdf_date_hdr_t { 
  uint8_t jtype;
  uint8_t qtype;
  uint8_t unused1;
  uint8_t unused2;
  uint32_t poff;
  tm_t tm_val;
} qdf_date_hdr_t;

typedef struct _qdf_string_hdr_t { 
  uint8_t jtype;
  uint8_t qtype;
  uint8_t unused1;
  uint8_t unused2;
  uint32_t poff;
  uint32_t str_len; // str_len+1 <= str_size +1 for nullc
  uint32_t str_size;
} qdf_string_hdr_t;

typedef struct _qdf_array_hdr_t { 
  uint8_t jtype;
  uint8_t qtype;
  uint16_t width;
  uint32_t poff;

  uint32_t arr_len; // arr_len <= arr_size
  uint32_t arr_size;

  uint32_t qdf_size;
  uint32_t unused1;

} qdf_array_hdr_t;

typedef struct _qdf_object_hdr_t { 
  uint8_t jtype;
  uint8_t qtype;
  uint8_t unused1;
  bool is_df; // whether is data frame 
  uint32_t poff;

  uint32_t obj_len; // number of keys in object 
  uint32_t obj_arr_len; // relevant only when is_df = true 

  uint32_t qdf_size;
  uint32_t obj_arr_size; // relevant only when is_df = true 

} qdf_object_hdr_t;

typedef struct _qdf_rec_type {
  void *data;
  uint32_t size;
  bool is_mmap; // we have mmapped data not malloc'd it 
  bool is_foreign; // true => do not delete 
  bool is_read_only; // true => don't modify
  char *name; // for debugging  TODO DELETE LATER 
  int id; // for debugging TODO DELETE LATER 
} QDF_REC_TYPE;
//STOP_FOR_CDEF


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
    char **vals, // [n_cols][sz_rows]
    uint32_t n_rows,
    uint32_t sz_rows,
    const qtype_t *const qtypes, // [n_svals]
    QDF_REC_TYPE *ptr_qdf
    );
extern int
make_mixed_array_or_object(
    QDF_REC_TYPE **qdf_vals, // [n_qdfs]
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
    const qtype_t * const qtypes, // [n_cols]
    const uint32_t * const widths, // [n_cols]
    uint32_t sz_rows,
    QDF_REC_TYPE *ptr_qdf
    );
extern int
place_in_data_frame(
    QDF_REC_TYPE *ptr_dst, // dataframe to be modified in place
    const  char * const key, // name  of  column
    QDF_REC_TYPE *ptr_src // column to be placed into dst
    );
extern int
set_is_df(
    void *x,
    bool  is_df
    );
extern bool
get_is_df(
    const void * const x
    );
extern jtype_t 
get_jtype(
    const void * const x
    );
extern qtype_t 
get_qtype(
    const void * const x
    );
extern uint32_t
get_poff(
    const void * const x
    );
extern uint32_t
get_str_len(
    const void * const x
    );
extern uint32_t
get_str_size(
    const void * const x
    );
extern uint32_t
get_arr_len(
    const void * const x
    );
extern uint32_t
get_arr_size(
    const void * const x
    );
extern uint32_t
get_obj_len(
    const void * const x
    );
extern double
get_num_val(
    const void * const x
    );
extern int
get_date_val(
    const void * const x,
    tm_t *ptr_tm
    );
extern char * 
get_str_val(
    void * x
    );
extern bool
get_bool_val(
    const void * const x
    );
extern int
free_qdf(
    QDF_REC_TYPE *ptr_qdf
    );
extern int
copy_qdf(
    QDF_REC_TYPE *ptr_dst,
    const QDF_REC_TYPE * const ptr_src
    );
extern int
get_arr_val(
    void * const x,
    uint32_t idx,
    SCLR_REC_TYPE *out_sclr,
    QDF_REC_TYPE *out_qdf
    );
extern int
get_keys(
    const QDF_REC_TYPE * const ptr_qdf_df,
    QDF_REC_TYPE *ptr_qdf_keys
    );
extern int
is_val_in_SC_array(
    const QDF_REC_TYPE * const ptr_qdf, // input 
    const char * const key, // input 
    int32_t *ptr_idx  // output 
    );
extern int
get_key_val(
    const QDF_REC_TYPE *const ptr_in,
    int nidx, // key described as index 
    const char * const sidx, // key described as string
    QDF_REC_TYPE *ptr_out,
    uint32_t *ptr_offset // how far the output col is from start of dataframe
   );
extern int
get_arr_idx(
    void * x, // input 
    SCLR_REC_TYPE *out, // input 
    int32_t *ptr_idx // output 
    );
extern int
is_key(
    const QDF_REC_TYPE *const ptr_in,
    const char * const sidx, // key described as string
    bool *ptr_is_key
   );
extern bool
is_nil(
    const QDF_REC_TYPE *const ptr_in
    );
extern void
zero_qdf(
    QDF_REC_TYPE *ptr_qdf
    );
extern uint32_t * 
get_offsets_in_arr(
    void * x
    );
extern void *
get_offsets_in_obj(
    void * x
    );
extern void *
get_keys_in_obj(
    void * x
    );
extern uint32_t
get_qdf_size(
    const void * x
    );
extern void
set_jtype(
    void *x,
    jtype_t jtype
    );
extern void
set_qtype(
    void *x,
    qtype_t  qtype
    );
extern int
set_str_len(
    void *x,
    uint32_t str_len
    );
extern int
set_str_size(
    void *x,
    uint32_t str_size
    );
extern int
set_num_val(
    void *x,
    double dval
    );
extern int
set_date_val(
    void *x,
    tm_t date_val
    );
extern int
set_str_val(
    void *x,
    const char * const str_val 
    );
extern int
set_bool_val(
    void *x,
    bool bval
    );
extern int
set_qdf_size(
    void *x,
    uint32_t qdf_size
    );
extern int
set_arr_width(
    void *x,
    uint32_t  width
    );
extern uint32_t
get_arr_width(
    const void * const x
    );
extern int
set_arr_len(
    void *x,
    uint32_t arr_len
    );
extern int
set_obj_len(
    void *x,
    uint32_t arr_len
    );
extern int
set_arr_size(
    void *x,
    uint32_t arr_size
    );
extern int
set_obj_arr_size(
    void *x,
    uint32_t arr_size
    );
extern uint32_t
get_obj_arr_size(
    const void * const x
    );
extern int
set_obj_arr_len(
    void *x,
    uint32_t arr_len
    );
extern uint32_t
get_obj_arr_len(
    const void * const x
    );
extern int 
get_sclr_as_F8(
    const SCLR_REC_TYPE *const ptr_sclr,
    double *ptr_x
    );
extern void * 
get_arr_ptr(
    void * x
    );
extern int 
get_keys_as_array(
    QDF_REC_TYPE *ptr_qdf,
    char ***ptr_keys,
    uint32_t *ptr_n_keys
    );
extern int 
get_str_qtypes_as_array(
    QDF_REC_TYPE *ptr_qdf,
    char ***ptr_str_qtypes, 
    uint32_t *ptr_n_str_qtypes
    );
extern char *
get_qdf_ptr(
    QDF_REC_TYPE *ptr_qdf
    );
extern int 
get_qdf_ptr_as_hex_string(
    QDF_REC_TYPE *ptr_qdf,
    char hex_str[32]
    );
extern uint32_t
get_qdf_raw_size(
    QDF_REC_TYPE *ptr_qdf
    );
extern int 
get_qtypes_as_array(
    QDF_REC_TYPE *ptr_qdf,
    qtype_t **ptr_qtypes, 
    uint32_t *ptr_n_qtypes
    );
extern int 
get_widths_as_array(
    QDF_REC_TYPE *ptr_qdf,
    uint32_t **ptr_widths, 
    uint32_t *ptr_n_widths
    );
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
int
x_set_foreign(
    QDF_REC_TYPE * ptr_qdf
    );
extern int
qdf_csv_to_df(
    const char * const infile, // INPUT 
    char *X, // INPUT (alternative to infile)
    size_t nX, // INPUT (alternative to infile)
    char ** const in_cols, // INPUT  [in_ncols]
    const qtype_t * const in_qtypes, // INPUT  [in_ncols]
    const uint32_t * const in_widths, // INPUT  [in_ncols]
    char ** const in_formats, // INPUT [in_ncols]
    const bool * const in_has_nulls, // INPUT  [in_ncols]
    const bool * const in_is_load, // INPUT  [in_ncols]
    uint32_t in_ncols,
    const char * const str_fld_sep, // INPUT 
    const char * const str_fld_delim, // INPUT 
    const char * const str_rec_sep, // INPUT 
    bool is_hdr, // INPUT 
    QDF_REC_TYPE *ptr_qdf // OUTPUT 
    );
extern int
qdf_const(
    SCLR_REC_TYPE *ptr_sclr,
    uint32_t n,
    uint32_t sz,
    QDF_REC_TYPE *dst // output 
   );
extern int
qdf_const_str(
    const char * const str,
    uint32_t n,
    uint32_t sz,
    QDF_REC_TYPE *dst // output 
    );
extern int
qdf_seq(
    SCLR_REC_TYPE *ptr_start,
    SCLR_REC_TYPE *ptr_incr,
    uint32_t n,
    uint32_t sz,
    QDF_REC_TYPE *dst // output 
   );
extern int
qdf_incr_on_by(
    const QDF_REC_TYPE *const ptr_src, // input
    const SCLR_REC_TYPE * const ptr_on,
    const SCLR_REC_TYPE * const ptr_incr,
    bool reset,
    QDF_REC_TYPE * restrict ptr_dst // output 
   );
extern int
qdf_is_sawtooth(
    const QDF_REC_TYPE *const ptr_src, // input
    bool *ptr_b
   );
extern int
qdf_grp_sawtooth(
    const QDF_REC_TYPE *const ptr_src, // input
    const QDF_REC_TYPE *const ptr_n_grp, // input
    const char * const str_dqtype,
    QDF_REC_TYPE * restrict ptr_dst // output 
   );
extern int
grp_sawtooth_I1_I1(
    const int8_t *const src, // [sn] 
    uint32_t sn,
    const int32_t * const n_grp, // [gn]
    uint32_t gn,
    int8_t *dst // [sn]
    );
extern int
qdf_grp_flatten_sawtooth(
    const QDF_REC_TYPE *const ptr_src, // input
    const QDF_REC_TYPE *const ptr_n_grp, // input
    QDF_REC_TYPE * restrict ptr_dst // output 
   );
extern int
grp_flatten_sawtooth_I1(
    const int8_t *const s, // [sn] 
    uint32_t sn,
    const int32_t * const n_grp, // [gn]
    uint32_t gn,
    int8_t *d // [sn]
    );
extern int
vals_counts(
    const QDF_REC_TYPE * const src,  // input
    QDF_REC_TYPE * restrict ptr_qdf_val, // output 
    QDF_REC_TYPE * restrict ptr_qdf_cnt // output 
   );
extern int
vals_sums(
    const QDF_REC_TYPE * const ptr_qdf_key,  // input
    const QDF_REC_TYPE * const ptr_qdf_num,  // input
    QDF_REC_TYPE * restrict ptr_qdf_val, // output 
    QDF_REC_TYPE * restrict ptr_qdf_sum // output 
   );
extern int
chk_qtype(
    qtype_t qtype
    );
extern int 
chk_mixed_array(
    char *x
    );
extern int
chk_qdf(
    const QDF_REC_TYPE * const ptr_qdf
    );
extern int
chk_hdr(
    void *x
    );
extern int
chk_jtype(
    jtype_t jtype,
    qtype_t qtype
    );
extern int 
chk_nil(
    void *x
    );
extern int 
chk_bool(
    void *x
    );
extern int 
chk_number(
    void *x
    );
extern int 
chk_date(
    void *x
    );
extern int 
chk_string(
    void *x
    );
extern int
chk_uniform_array(
    void *x,
    qtype_t qtype
    );
extern int 
chk_object(
    void *x
    );
extern int
chk_is_data_frame(
      const QDF_REC_TYPE *const ptr_qdf,
      bool *ptr_is_df
    );
extern int
are_keys_in_object(
      const QDF_REC_TYPE * ptr_qdf,
      char ** const keys, // [n_keys]
      uint32_t n_keys,
      bool *ptr_are_keys
    );
extern int
qdf_get_by_idx(
    const QDF_REC_TYPE * const ptr_qdf, // input 
    uint32_t idx, // input 
    SCLR_REC_TYPE *ptr_sclr // output 
   );
extern int
compress(
    const QDF_REC_TYPE *const src,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2,
    compress_mode_t *ptr_compress_mode
    );
extern int
compress_distinct_vals(
    const QDF_REC_TYPE *const src,
    qtype_t src_qtype,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_distinct_vals_F4(
    const QDF_REC_TYPE *const src,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_distinct_vals_I2(
    const QDF_REC_TYPE *const src,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_distinct_vals_I4(
    const QDF_REC_TYPE *const src,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_F4_to_F2(
    const QDF_REC_TYPE *const src,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
compress_none(
    const QDF_REC_TYPE * const src,
    QDF_REC_TYPE * dst1,
    QDF_REC_TYPE * dst2
    );
extern int
compress_week_to_offset(
    const QDF_REC_TYPE *const src,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    );
extern int
decompress(
    const QDF_REC_TYPE * const src1,
    const QDF_REC_TYPE * const src2,
    compress_mode_t compress_mode,
    QDF_REC_TYPE *dst
    );
extern int
decompress_distinct_vals(
    const QDF_REC_TYPE * const src1,
    const QDF_REC_TYPE * const src2,
    qtype_t qtype,
    QDF_REC_TYPE *dst
    );
extern int
decompress_F4_to_F2(
    const QDF_REC_TYPE * const src,
    QDF_REC_TYPE *dst
    );
extern int
mk_sort_idx_I1_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I1_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I1_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I1_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I1_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I1_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I2_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I2_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I2_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I2_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I2_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I2_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I4_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I4_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I4_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I4_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I4_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I4_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I8_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I8_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I8_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I8_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I8_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_I8_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F4_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F4_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F4_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F4_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F4_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F4_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F8_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F8_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F8_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F8_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F8_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
mk_sort_idx_F8_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
extern int
permute(
    const QDF_REC_TYPE * const src, 
    const QDF_REC_TYPE * const idx, 
    const char * const direction,
    QDF_REC_TYPE *dst // output 
   );
extern int
fill(
    const QDF_REC_TYPE * const src, 
    const QDF_REC_TYPE * const nn, 
    const QDF_REC_TYPE * const grp, 
    const QDF_REC_TYPE * const tim, 
    QDF_REC_TYPE *dst // output 
   );
extern int
srt_grp_cnt(
    const QDF_REC_TYPE * const src, 
    QDF_REC_TYPE *dst // output 
   );
extern QDF_REC_TYPE *
qdf_cmem_make(
    uint32_t sz,
    const char * const name // for debugging 
    );
extern QDF_REC_TYPE *
qdf_cmem_clone(
    QDF_REC_TYPE *x
    );
extern QDF_REC_TYPE *
qdf_cmem_dupe(
    QDF_REC_TYPE *y
    );
extern int 
qdf_cmem_free(
  QDF_REC_TYPE *x
);
extern void
qdf_cmem_nop(
  QDF_REC_TYPE *x
);
extern int 
qdf_cmem_set_name(
  QDF_REC_TYPE *x,
  const char * const name
);
extern char * 
qdf_cmem_get_name(
  QDF_REC_TYPE *x
);
extern int
vs_op(
    QDF_REC_TYPE * src, // input numeric array 
    const SCLR_REC_TYPE * const ptr_sclr,
    const char * const lop,
    QDF_REC_TYPE *dst // output numeric array I1
   );
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
extern int
vv_op(
    QDF_REC_TYPE * src1, // input numeric array 
    const QDF_REC_TYPE * const src2, // input numeric array 
    const char * const lop,
    QDF_REC_TYPE *ptr_dst // output numeric array 
   );
extern int
v_op(
    QDF_REC_TYPE *src, // input numeric array 
    const char * const op,
    QDF_REC_TYPE *dst // output numeric array 
   );
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
extern int
x_set_arr_val(
    QDF_REC_TYPE *ptr_qdf,
    uint32_t idx,
    SCLR_REC_TYPE sclr
    );
extern int
x_set_arr_all(
    QDF_REC_TYPE *ptr_qdf, 
    const char * const change_col, 
    const char * const change_val
    );
extern int
squeeze_where(
    QDF_REC_TYPE * restrict dst, // object consisting of numeric arrays
    // or a numeric array 
    const QDF_REC_TYPE *const where, // numeric I1 array (1=> good, 0 => null)
    uint32_t *ptr_num_good
   );
extern int
qdf_where(
    QDF_REC_TYPE *ptr_src, // INPUT df consisting of numeric arrays
    const QDF_REC_TYPE *const where, // numeric I1 array (1=> good, 0 => null)
    QDF_REC_TYPE *ptr_dst, // OUTPUT: same as INPUT 
    uint32_t *ptr_num_good
   );
extern int
qdf_nullify(
    QDF_REC_TYPE *ptr_qdf, // data frame containing col to be null'ed
    const char * const col_name
    );
extern int
pr_df_as_csv(
    const QDF_REC_TYPE * const qdf,
    char **  const keys,
    uint32_t n_keys,
    const char * const file_name,
    bool as_html
    );
extern int
pr_json(
    const QDF_REC_TYPE * const ptr_qdf,
    QDF_REC_TYPE * restrict ptr_out_str, // where output string is created
    uint32_t *ptr_buflen, // how much of allocated space has been used 
    FILE *fp
    );
extern int
pr_1(
    const void * const valptr, 
    const char * const nn_ptr,
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
extern int
x_pr_json(
    const QDF_REC_TYPE * const ptr_qdf,
    const char * const file_name
    );
extern int
pr_df_as_html(
    const QDF_REC_TYPE * const ptr_qdf,
    char ** const disp_keys, // [n_disp] Logical name: for display 
    char ** const in_viz_keys, // [n_disp] Display name
    char ** const in_id_keys, // [n_disp] id for JS/HTML purposes
    bool *is_editable, // [n_disp]
    uint32_t n_disp,
    char ** const edit_keys, // keys to be editable 
    uint32_t n_edit,
    bool is_all_non_editable, 
    bool is_all_editable, 
    const char * const table_id, 
    const char * const caption,
    const char * const file_name,
    char **ptr_outbuf,
    size_t *ptr_outlen,
    bool as_str // disregard file_name in this case
    );
extern int
prefix_sums(
    const QDF_REC_TYPE *const src,
    const char * const direction,
    QDF_REC_TYPE *restrict dst
    );
extern int 
qdf_resize_df(
    QDF_REC_TYPE *ptr_old,
    char ** in_keys, // [in_n_keys]
    char ** in_str_qtypes, // [in_n_keys]
    uint32_t in_n_keys,
    uint32_t new_sz,
    QDF_REC_TYPE *ptr_new
    );
extern int
qdf_append_df(
    QDF_REC_TYPE * restrict dst,
    const QDF_REC_TYPE *const src,
    uint32_t *ptr_num_to_expand 
   );
extern int
qdf_copy_array(
    QDF_REC_TYPE *dst,
    const QDF_REC_TYPE * const src
   );
extern int
coerce(
    void *dst, 
    qtype_t dst_qtype,
    const void * const src,
    qtype_t src_qtype,
    uint32_t sn
    );
extern int
qdf_clone(
    const QDF_REC_TYPE * const src,
    QDF_REC_TYPE * dst
   );
extern int 
qdf_join(
    QDF_REC_TYPE *ptr_from_key,
    QDF_REC_TYPE *ptr_from_idx,
    QDF_REC_TYPE *ptr_from_val,

    QDF_REC_TYPE *ptr_to_key,
    QDF_REC_TYPE *ptr_to_idx,
    QDF_REC_TYPE *ptr_to_val,
    QDF_REC_TYPE *ptr_nn_to_val
    ); 
extern int
smear(
    const QDF_REC_TYPE *const src,
    const QDF_REC_TYPE *const off, // offset 
    const char * const direction,
    QDF_REC_TYPE *restrict dst
    );
extern int
qdf_sort(
    QDF_REC_TYPE *src, // input and output (modified in place)
    const char * const mode
   );
extern int 
free_qdfs(
    QDF_REC_TYPE **ptr_qdfs,  // [n] 
    const char * const mode,
    uint32_t n
    );
extern int
load_qdfs_from_files(
    char ** const infiles,  // [n_in] INPUT 
    uint32_t n_in,  // INPUT 
    bool is_writable, // INPUT 
    QDF_REC_TYPE **ptr_qdfs  // OUTPUT 
    );
extern int
bindmp(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const file_name
    );
extern int
binld(
    QDF_REC_TYPE *ptr_qdf,
    const char * const file_name,
    bool is_writable
    );
extern int
shift_up_down(
    const QDF_REC_TYPE * const src, // input numeric array 
    uint32_t shift_by,
    const char * const direction,
    QDF_REC_TYPE * restrict dst, // output numeric array (same type as src)
    QDF_REC_TYPE * restrict nn_dst // output numeric array I1
   );
typedef enum { 
  cmp_undef,
  cmp_eq,
  cmp_neq,
  cmp_lt,
  cmp_leq,
  cmp_gt,
  cmp_geq
} cmp_t;

typedef enum { 
  next_prev_undef,
  is_next,
  is_prev,
} next_prev_t;

extern int
qdf_next_prev(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const str_op,
    const char * const str_cmp,
    bool *ptr_rslt
    );
extern bool
calc_is_next(
    const void * const X, 
    uint32_t n, 
    qtype_t qtype, 
    cmp_t cmp
    );
extern int
tm_extract(
    const QDF_REC_TYPE * const ptr_src,
    const char * const fld,
    QDF_REC_TYPE * restrict ptr_dst
    );
extern int
x_time_band(
    const QDF_REC_TYPE *const ptr_qdf,
    uint32_t n_recent,
    QDF_REC_TYPE * restrict ptr_out_qdf
    );
extern int
qdf_mktime(
    const QDF_REC_TYPE *const ptr_src,
    QDF_REC_TYPE * restrict ptr_dst
    );
extern int
qdf_I4_to_TM1(
    const QDF_REC_TYPE *const ptr_src,
    QDF_REC_TYPE * restrict ptr_dst
    );
extern int
qdf_mk_df(
    QDF_REC_TYPE *ptr_qdf
    );
extern int
qdf_write(
    const QDF_REC_TYPE * const src, // INPUT 
    uint32_t offset, // INPUT
    bool truncate, // INPUT
    QDF_REC_TYPE * restrict dst // INPUT and OUTPUT 
   );
extern int
add_col_to_df(
    QDF_REC_TYPE *src,
    const char * const col, // name of new column
    QDF_REC_TYPE *add, // column to be added 
    QDF_REC_TYPE *dst
    );
extern int
x_num_unique(
    const QDF_REC_TYPE *const ptr_qdf,
    const QDF_REC_TYPE *const ptr_nn_qdf,
    uint32_t * ptr_n_uq
    );
extern int
x_is_unique(
    const QDF_REC_TYPE *const ptr_qdf,
    const QDF_REC_TYPE *const ptr_nn_qdf,
    bool *ptr_is_uq
    );
extern int
x_num_unique_saturated(
    const QDF_REC_TYPE *const ptr_qdf,
    uint32_t max_n_uq,
    uint32_t * ptr_n_uq
    );
extern int
nop3(
    SCLR_REC_TYPE *x
    );
extern int
nop2(
    PRIMITIVE_VAL_TYPE *x
    );
extern int
nop(
    QDF_REC_TYPE *x
    );
]]
