extern int
x_num_unique(
    const QDF_REC_TYPE *const ptr_qdf,
    const QDF_REC_TYPE *const ptr_nn_qdf,
    uint32_t * ptr_n_uq
    );
extern int
x_is_unique(
    const QDF_REC_TYPE *const ptr_qdf,
    const QDF_REC_TYPE *const ptr_nn_qdf,
    bool *ptr_is_uq
    );
extern int
x_num_unique_saturated(
    const QDF_REC_TYPE *const ptr_qdf,
    uint32_t max_n_uq,
    uint32_t * ptr_n_uq
    );
