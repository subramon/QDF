// TODO P2 Add is_load to read_csv and to this function 
// input is a CSV file, output is a QDF dataframe
extern int
qdf_csv_to_df(
    const char * const infile, // INPUT 
    const char * const concat_cols, // INPUT 
    const char * const concat_qtypes, // INPUT 
    const char * const str_fld_sep, // INPUT 
    const char * const str_fld_delim, // INPUT 
    const char * const str_rec_sep, // INPUT 
    bool is_hdr, // INPUT 
    BUF_SPEC_TYPE *ptr_buf_spec, // INPUT 
    QDF_REC_TYPE *ptr_qdf // OUTPUT 
    );
