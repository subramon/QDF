extern int
set_lags(
    QDF_REC_TYPE *dst, // dataframe to be modified
    int start_lag, // in weeks
    int stop_lag, // in weeks
    const char * const lag_col_prefix, // e.g., sls_unit_q_l
    // so the columns set will be sls_unit_q_l1, sls_unit_q_l2, ...
    // aassuming that start_lag is 1
    QDF_REC_TYPE *src_grpby, // contains input value as I4
    QDF_REC_TYPE *src_val, // contains input value as F4
    QDF_REC_TYPE *src_tim // contains input time  as TM
   );
