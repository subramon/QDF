typedef struct _qdf_df_meta_data_t { 
  char **col_names; // [n_cols]
  uint32_t n_cols;
  qtype_t *c_qtypes;    // [n_cols] e.g., F4
  bool *has_nulls;  // [n_cols]
  bool *is_load;    // [n_cols] 
  // when used to describe meta data of data frame sent to R
  // is_load[i] == false => do not send i^{th} column to R
} qdf_df_meta_data_t;

extern int 
qdf_to_Rserve(
    int sock,
    QDF_REC_TYPE *ptr_qdf,
    const qdf_df_meta_data_t *meta,
    const char * const df_name
);
