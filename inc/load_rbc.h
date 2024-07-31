extern int 
load_qdf(
        char ** const str_break_cols, // from C->ddl_in
        uint32_t n_break_cols, // from C->ddl_in
        const char * const dir,
        const char * const prefix,
        dsid_t dsid,
        bool is_writable,
        QDF_REC_TYPE *ptr_out_qdf
        );
