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
