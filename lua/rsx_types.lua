local ffi = require 'ffi'
ffi.cdef([[
typedef struct _dfeds_rs_hmap_key_t { 
  int32_t id[2]; 
} dfeds_rs_hmap_key_t;
typedef struct _dfeds_rs_hmap_val_t {
  uint32_t num_rows_read;
  uint32_t num_rows_dropped;
  float t_plp1;  // time taken for plp1 in micro-seconds
  float t_plp2;  // time taken for plp2 in micro-seconds

  uint64_t plp2_err_bmask; // 0 => no error  
  // The 64 bits in plp2_bmask are used as follows
  // each formula uses 4 bits => max of 16 bits
  // each "nibble" has the plp2_error => max of 16-1 error codes
  // 0 => no error 
  uint8_t plp1_error; // 0 => no error  
  uint8_t server_id; // which server built this model 

  uint8_t skip_frmla_bmask; // Explanation as follows
  /* If ith bit is 1 => we decided to skip this formula */
  /* If ith bit is 0 => we decided to evaluate this formula */

  uint8_t succ_frmla_bmask; // Explanation as follows
  //ith bit is 0 => we tried and failed to build a model for formula i 
  //ith bit is 1 => we tried and succeeded in building a model for formula i

  uint8_t num_models_attempted; 
  uint8_t junk1; // so that we end up on a 32-bit word boundary

  float t_model_building; // measured in micro-seconds
  uint32_t t_data_created;  // time in seconds since epoch
  uint32_t t_model_created;  // time in seconds since epoch
} dfeds_rs_hmap_val_t;
]]
)
