extern int
prep_args_for_read_csv(
    // input 
    char * const in_cols,
    char * const in_str_qtypes,
    // output 
    qtype_t **ptr_qtypes,
    uint32_t **ptr_widths,
    char ***ptr_format,
    bool **ptr_has_nulls,
    bool **ptr_is_load ,
    char ***ptr_cols,
    uint32_t *ptr_ncols
    );
