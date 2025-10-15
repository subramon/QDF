extern int
pr_df_as_html(
    const QDF_REC_TYPE * const ptr_qdf,
    char ** const disp_keys, // keys to be displayed 
    char ** const ui_keys, // UI name of key to be displayed 
    uint32_t n_disp,
    char ** const edit_keys, // keys to be editable 
    uint32_t n_edit,
    bool all_non_editable, 
    bool all_editable, 
    const char * const table_id, 
    const char * const caption,
    const char * const file_name
    );
