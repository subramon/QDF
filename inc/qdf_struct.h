#ifndef QDF_STRUCT_H
#define QDF_STRUCT_H
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "qdf_consts.h"
#include "qtypes.h"
#include "sclr_struct.h"
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
  bool junk; // just for padding 
  char *name; // for debugging  TODO DELETE LATER 
  int id; // for debugging TODO DELETE LATER 
} QDF_REC_TYPE;
//STOP_FOR_CDEF

#define mcr_get_jtype(x) { ((qdf_hdr_t *)x)->jtype }
#define mcr_get_qtype(x) { ((qdf_hdr_t *)x)->qtype }
#define mcr_get_arr_len(x) { ((qdf_array_hdr_t *)x)->arr_len }
#define mcr_get_arr_width(x) { ((qdf_array_hdr_t *)x)->arr_width }

#endif // QDF_STRUCT_H
