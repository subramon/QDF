 return [[ 

extern int
read_csv(
    const char * const infile,
    char *in_X,
    size_t in_nX,

    char ** const qtypes,
    char ** const out,
    const uint32_t * const width,
    uint32_t nrows,
    uint32_t ncols,
    const char * const str_fld_sep,
    const char * const str_fld_delim,
    const char * const str_rec_sep,
    bool is_hdr
    );

 ]] 
