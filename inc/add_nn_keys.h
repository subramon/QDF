extern int
add_nn_keys(
    char **keys, // [nK]
    const qtype_t * const qtypes, // [nK]
    const uint32_t *const widths, // nK] 
    bool *has_nulls, // [nK]
    bool *is_load, // [nK]
    uint32_t nK,
    // output below 
    char ***ptr_all_keys, // [all_nK]
    bool **ptr_all_qtypes, // [all_nK]
    uint32_t **ptr_all_widths, // [all_nK]
    uint32_t *ptr_all_nK
    );
