#include "incs.h"
#include "get_time_usec.h"
#include "dfeds.h"

extern dfeds_rs_hmap_t g_dfeds_hmap;

int
dfeds_is(
    dfeds_rs_hmap_key_t *key,
    bool *ptr_is_found,
    uint32_t *ptr_where_found
    )
{
  int status = 0;
  dfeds_rs_hmap_val_t val; memset(&val, 0, sizeof(dfeds_rs_hmap_val_t));
  if ( key == NULL ) { go_BYE(-1); }
  status = g_dfeds_hmap.get(&g_dfeds_hmap, key, 
      &val, ptr_is_found, ptr_where_found);
  cBYE(status);
BYE:
  return status;
}
int
dfeds_add_to_db(
    ds_asm_output_t *D
    )
{
  int status = 0;
  bool is_found; uint32_t where_found;
  dfeds_rs_hmap_val_t v; memset(&v, 0, sizeof(dfeds_rs_hmap_val_t));
  if ( D == NULL ) { go_BYE(-1); } 

  // D->dsid is the key 
  status = dfeds_is(&(D->dsid), &is_found, &where_found); cBYE(status);
  if ( is_found ) { 
    dfeds_rs_hmap_val_t *dst_v = &(g_dfeds_hmap.bkts[where_found].val);
    dst_v->num_rows_read = D->num_rows_read;
    dst_v->num_rows_dropped = D->num_rows_dropped;
    dst_v->t_data_created = (uint32_t)(get_time_usec() / 1000000);
    // important that following are reset
    dst_v->t_model_created  = 0;  
    dst_v->plp1_error       = 0;  
    dst_v->plp2_err_bmask   = 0;  
    dst_v->skip_frmla_bmask = 0;  
    dst_v->succ_frmla_bmask = 0;  
    //----------------
  }
  else { 
    v.num_rows_read = D->num_rows_read;
    v.num_rows_dropped = D->num_rows_dropped;
    v.t_data_created = (uint32_t)(get_time_usec() / 1000000);
    status = g_dfeds_hmap.put(&g_dfeds_hmap, &(D->dsid), &v); cBYE(status);
#ifdef DEBUG
    status = dfeds_is(&(D->dsid), &is_found, &where_found); cBYE(status);
    if ( !is_found ) { go_BYE(-1); } 
    if ( where_found >= g_dfeds_hmap.size ) { go_BYE(-1); } 
#endif
  }
BYE:
  return status;
}

int
dfeds_set_fld(
    dsid_t dsid,
    const char * const fld,
    QDF_SCLR_TYPE *ptr_sclr
    )
{
  int status = 0;
  bool is_found; uint32_t where_found;
  dfeds_rs_hmap_val_t v; memset(&v, 0, sizeof(dfeds_rs_hmap_val_t));

  // D->dsid is the key 
  status = dfeds_is(&dsid, &is_found, &where_found); cBYE(status);
  if ( !is_found ) { go_BYE(-1); } 

  dfeds_rs_hmap_val_t *dst_v = &(g_dfeds_hmap.bkts[where_found].val);
  if ( strcmp(fld, "t_plp1") == 0 ) { 
    if ( ptr_sclr->qtype != F4 ) { go_BYE(-1); } 
    dst_v->t_plp1 = ptr_sclr->val.f4;
  }
  else if ( strcmp(fld, "t_plp2") == 0 ) { 
    if ( ptr_sclr->qtype != F4 ) { go_BYE(-1); } 
    dst_v->t_plp2 = ptr_sclr->val.f4;
  }
  else if ( strcmp(fld, "plp2_err_bmask") == 0 ) { 
    if ( ptr_sclr->qtype != I8 ) { go_BYE(-1); } 
    dst_v->plp2_err_bmask = (uint64_t)ptr_sclr->val.i8;
  }
  else if ( strcmp(fld, "server_id") == 0 ) { 
    if ( ptr_sclr->qtype != I1 ) { go_BYE(-1); } 
    dst_v->plp1_error = (uint8_t)ptr_sclr->val.i1;
  }
  else if ( strcmp(fld, "plp1_error") == 0 ) { 
    if ( ptr_sclr->qtype != I1 ) { go_BYE(-1); } 
    dst_v->plp1_error = (uint8_t)ptr_sclr->val.i1;
  }
  else if ( strcmp(fld, "skip_frmla_bmask") == 0 ) { 
    if ( ptr_sclr->qtype != I1 ) { go_BYE(-1); } 
    dst_v->skip_frmla_bmask = (uint8_t)ptr_sclr->val.i1;
  }
  else if ( strcmp(fld, "succ_frmla_bmask") == 0 ) { 
    if ( ptr_sclr->qtype != I1 ) { go_BYE(-1); } 
    dst_v->succ_frmla_bmask = (uint8_t)ptr_sclr->val.i1;
  }
  else if ( strcmp(fld, "num_models_attempted") == 0 ) { 
    if ( ptr_sclr->qtype != I1 ) { go_BYE(-1); } 
    dst_v->num_models_attempted = (uint8_t)ptr_sclr->val.i1;
  }
  else if ( strcmp(fld, "t_model_building") == 0 ) { 
    if ( ptr_sclr->qtype != F4 ) { go_BYE(-1); } 
    dst_v->t_model_building = ptr_sclr->val.f4;
  }
  else if ( strcmp(fld, "t_data_created") == 0 ) { 
    if ( ptr_sclr->qtype != I4 ) { go_BYE(-1); } 
    dst_v->t_data_created = (uint32_t)ptr_sclr->val.i4;
  }
  else if ( strcmp(fld, "t_model_created") == 0 ) { 
    if ( ptr_sclr->qtype != I4 ) { go_BYE(-1); } 
    dst_v->t_model_created = (uint32_t)ptr_sclr->val.i4;
  }
  else if ( strcmp(fld, "t_fcast_created") == 0 ) { 
    if ( ptr_sclr->qtype != I4 ) { go_BYE(-1); } 
    dst_v->t_fcast_created = (uint32_t)ptr_sclr->val.i4;
  }
  else { 
    fprintf(stderr, "Unknown field [%s]\n", fld);
    go_BYE(-1);
  }
BYE:
  return status;
}
int
dfeds_pr(
    const char * const dir,
    const char * const opfile
    )
{
  int status = 0;
  FILE *fp = NULL;
  uint32_t len = 0; char * str = NULL;

  if ( dir == NULL ) { go_BYE(-1); }
  if ( opfile == NULL ) { go_BYE(-1); }
  len = (uint32_t)(strlen(dir) + strlen(opfile) + 8);
  str = malloc(len); memset(str, 0, len);
  sprintf(str, "%s/%s", dir, opfile);

  fp = fopen(str, "w");
  return_if_fopen_failed(fp, str, "w");
  bool *bkt_full = g_dfeds_hmap.bkt_full; 
  for ( uint32_t i = 0; i < g_dfeds_hmap.size; i++ ) { 
    if ( bkt_full[i] == false ) { continue; }
    dfeds_rs_hmap_val_t *v = &(g_dfeds_hmap.bkts[i].val); 
    dfeds_rs_hmap_key_t *k = &(g_dfeds_hmap.bkts[i].key); 
    //---- custom code below 
    // print key 
    uint32_t is_str = k->is_str;
    for ( int b = 0; b < MAX_NUM_BREAK_VALS; b++ ) {
      if ( ( ( is_str >> b ) & 0x1 ) == 1 )  {
        fprintf(fp, "%s,", k->sid[b]);
      }
      else {
        fprintf(fp, "%u,", k->uid[b]);
      }
    }
    // print val 
    fprintf(fp, "%u,", v->num_rows_read);
    fprintf(fp, "%u,", v->num_rows_dropped);

    fprintf(fp, "%f,", v->t_plp1);
    fprintf(fp, "%f,", v->t_plp2);
    fprintf(fp, "%" PRIu64 ",", v->plp2_err_bmask);

    fprintf(fp, "%u,",  v->plp1_error);
    fprintf(fp, "%u,",  v->server_id);
    fprintf(fp, "%u,",  v->skip_frmla_bmask);
    fprintf(fp, "%u,",  v->succ_frmla_bmask);
    fprintf(fp, "%u,",  v->num_models_attempted);

    fprintf(fp, "%f,", v->t_model_building);
    fprintf(fp, "%u,", v->t_data_created);  
    fprintf(fp, "%u\n", v->t_model_created);  // Note no comma

  }


BYE:
  free_if_non_null(str);
  fclose_if_non_null(fp);
  return status;
}
