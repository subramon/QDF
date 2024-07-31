#include "qdf_struct.h"
#include "configs.h"
#include "mgcv_wrk_struct.h"
#include "R_scor_struct.h"
extern int
qdf_mgcv_submodel(
    const component_t * const C, // [nC] INPUT
    uint32_t nC, // INPUT
    const mgcv_model_struct_t * const M, // INPUT
    QDF_REC_TYPE * restrict dst // OUTPUT 
   );
extern int
qdf_mgcv_model(
    const config_t * const config, // INPUT 
    dsid_t dsid,
    const mgcv_model_struct_t * const M, // [nF] INPUT
    QDF_REC_TYPE * restrict out // OUTPUT 
   );
extern int
rehydrate_mgcv_model(
    const QDF_REC_TYPE * const qdf, // INPUT 
    const config_t * const config, // INPUT 
    mgcv_model_struct_t ** restrict ptr_M // [nF] OUTPUT
    );
extern int
qdf_make_meta(
    const config_t * const C, 
    dsid_t dsid,
    QDF_REC_TYPE * restrict dst
    );
extern int 
qdf_make_dsid(
    dsid_t dsid,
    char ** str_dsid, // [n_dsid]
    uint32_t n_dsid,
    QDF_REC_TYPE * restrict dst
    );
extern int
qdf_mgcv_fcast(
    const config_t * const config,
    dsid_t dsid,
    const R_scor_output_t * const S, // [nF] INPUT
    uint32_t fidx,
    QDF_REC_TYPE * restrict out // OUTPUT 
   );
extern int
qdf_multi_fcast(
    const config_t * const C,
    dsid_t dsid,
    const R_scor_output_t * const S, // [nF] INPUT
    QDF_REC_TYPE * restrict out
   );
extern int
qdf_make_model_6(
    model_class_6_t *ptr_m6,
    QDF_REC_TYPE *ptr_dst
    );
