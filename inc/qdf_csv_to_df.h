extern int
qdf_csv_to_df(
    const char * const infile, // INPUT 
    char *X, // INPUT (alternative to infile)
    size_t nX, // INPUT (alternative to infile)
    char ** const in_cols, // INPUT  [in_ncols]
    const qtype_t * const in_qtypes, // INPUT  [in_ncols]
    const uint32_t * const in_widths, // INPUT  [in_ncols]
    char ** const in_formats, // INPUT [in_ncols]
    const bool * const in_has_nulls, // INPUT  [in_ncols]
    const bool * const in_is_load, // INPUT  [in_ncols]
    uint32_t in_ncols,
    const char * const str_fld_sep, // INPUT 
    const char * const str_fld_delim, // INPUT 
    const char * const str_rec_sep, // INPUT 
    bool is_hdr, // INPUT 
    QDF_REC_TYPE *ptr_qdf // OUTPUT 
    );
