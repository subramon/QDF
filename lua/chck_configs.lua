-- TODO P1 local plpath = require 'pl.path'
-- TODO P1 require 'strict'
local qtypes      = require 'qtypes'
local trim_qtype  = require 'trim_qtype'
local chck_meta   = require 'chck_meta'
local chck_ddl_in = require 'chck_ddl_in'

local function chk_not_equal(x, y)
  if ( x and y ) then assert(x ~= y) end 
  return true 
end
local function chck_configs(C)
  assert(type(C) == "table")
  
  assert(type(C.cfg_dir) == "string")
  assert(#C.cfg_dir > 0) -- TODO P4 check that it is a directory

  if ( C.str_cutoff_date ) then 
    assert(type(C.str_cutoff_date) == "string")
    assert(#C.str_cutoff_date > 0)
  end
  --================================================
  local rM = C.out_dfe
  if ( rM ) then
    assert(type(rM) == "table")
    assert(#rM > 0)
    for i1, v1 in ipairs(rM) do 
      assert(type(v1.name) == "string")
      assert(type(v1.qtype) == "string")
      assert( (v1.qtype == "I4" ) or (v1.qtype == "F8" ) or 
        (v1.qtype == "SC" ))
      for i2, v2 in ipairs(rM) do 
        if ( i1 ~= i2 ) then
          assert(v1.name ~= v2.name)
        end
      end
    end
  end
  --================================================

  local M = C.in_meta
  assert(chck_meta(M))

  assert(chck_ddl_in(C.ddl_in, M))

  local oM = C.out_meta
  assert(chck_meta(oM))

  for i, m in ipairs(oM) do
    local qtype = m.qtype
    qtype = trim_qtype(qtype)
    -- TODO P3 Note limitation below 
    assert( ( qtype == "I1" ) or ( qtype == "I2" ) or
            ( qtype == "I4" ) or ( qtype == "HL" ) or
            ( qtype == "TM" ) or 
            ( qtype == "F4" ) or ( qtype == "F8" ) ) 
  end
  --=============================
  -- following for backward compatibility
  C.break_cols = C.in_meta.data_set_identifier 
  --=====================
  local Rinfo = C.rserve
  if ( Rinfo.str_R_setup ) then 
    assert(type(Rinfo.str_R_setup) == "string")
    assert(#Rinfo.str_R_setup > 0)
  end
  assert(type(Rinfo.snd_timeout_sec) == "number")
  assert(Rinfo.snd_timeout_sec >= 0) -- 0 means do not use 

  assert(type(Rinfo.rcv_timeout_sec) == "number")
  assert(Rinfo.rcv_timeout_sec >= 0) -- 0 means do not use 

  assert(type(Rinfo.max_num_servers) == "number")
  assert(Rinfo.max_num_servers >= 1) 

  assert(type(C.row_prod_qsz) == "number")
  assert(C.row_prod_qsz > 0)

  assert(type(C.ds_asm_qsz) == "number")
  assert(C.ds_asm_qsz > 0)

  assert(type(C.R_prep_qsz) == "number")
  assert(C.R_prep_qsz > 0)

  assert(type(C.R_call_qsz) == "number")
  assert(C.R_call_qsz > 0)

  assert(type(C.ds_prod_qsz) == "number")
  assert(C.ds_prod_qsz > 0)

  assert(type(C.R_work_qsz) == "number")
  assert(C.R_work_qsz > 0)

  assert(type(C.R_scor_qsz) == "number")
  assert(C.R_scor_qsz > 0)

  if ( not C.data_set_expansion ) then
    C.data_set_expansion = 1 
  end
  assert(type(C.data_set_expansion) == "number")
  assert(C.data_set_expansion > 1)

  assert(type(C.max_num_workers) == "number")
  assert(C.max_num_workers > 0)

  assert(type(C.max_num_Rcallers) == "number")
  assert(C.max_num_Rcallers > 0)

  assert(type(C.max_num_scorers) == "number")
  assert(C.max_num_scorers > 0)

  local servers = assert(Rinfo.servers)
  local ns = 0; -- number of servers
  local num_open_conn = 0
  local num_used_conn = 0
  for _, v in ipairs(servers) do 
    assert(type(v.address) == "string")
    assert(#v.address > 0)
    assert(type(v.port) == "number")
    assert(v.port > 0)
    assert(type(v.max_conn) == "number")
    assert(v.max_conn > 0)
    ns = ns + 1
  end
  assert(ns > 0)

  for k1, v1 in ipairs(servers) do 
    for k2, v2 in ipairs(servers) do 
      if ( k1 ~= k2 ) then
        if ( ( v1.port == v2.port ) and 
             ( v1.address == v2.address ) ) then
             print(k1, k2, v1.address, v2.address)
             error("Duplicate servers")
        end
      end
    end
  end

  --===============================
  local SD = C.save_info.dbinfo
  if ( SD ) then 
    assert(type(SD) == "table")
    assert(type(SD.host)    == "string"); assert(#SD.host > 0)
    assert(type(SD.user)    == "string"); assert(#SD.user > 0)
    assert(type(SD.passwd)  == "string"); assert(#SD.passwd > 0)
    assert(type(SD.db)      == "string"); assert(#SD.db > 0)
    assert(type(SD.port)    == "number"); assert(SD.port > 0)
  end
  --=== START for join cols 
  if ( C.join_cols ) then 
    assert(type(C.join_cols) == "table")
    assert(#C.join_cols > 0)
    assert(#C.join_cols == 2) -- TODO P3 assumption for now 
    -- join cols must be in input data 
    local l_qtypes = {}
    for k, v in ipairs(C.join_cols) do 
      local found = false
      for k2, v2 in ipairs(M) do 
        if ( v == v2.name ) then 
          found = true; 
          l_qtypes[k] = v2.qtype 
          local x, y = string.find(v2.qtype, "TM:")
          if ( x == 1 ) then 
            l_qtypes[k] = "TM"
          end
          break 
        end
      end
      assert(found == true)
    end
    -- no duplicates 
    for k1, v1 in ipairs(C.join_cols) do 
      for k2, v2 in ipairs(C.join_cols) do 
        if ( k1 ~= k2 ) then assert(v1 ~= v2) end 
      end
    end
    -- TODO P3 Current assumption
    assert ( ( ( l_qtypes[1] == "TM" ) and ( l_qtypes[2] == "I2" ) ) or 
             ( ( l_qtypes[1] == "I2" ) and ( l_qtypes[2] == "TM" ) ) )
    
  end

  --===  STOP for join cols 
  --=== START for week_start_date to holiday_bmask
  if ( C.week_hol_file )  then 
    assert(type(C.week_hol_file) == "string")
    local lightR_root = assert(os.getenv("LIGHTR_ROOT"), "LIGHTR_ROOT not defined")
    local filename = 
      lightR_root ..  "/setup/" .. C.cfg_dir .. "/" ..  C.week_hol_file

    -- TODO P1 assert(plpath.exists(filename), filename)
    C.week_hol_file = filename 
    -- TODO P1 assert(plpath.exists(C.week_hol_file))
    assert(type(C.week_hol_time_format) == "string")
  end
  --=== STOP  for week_start_date to holiday_bmask
  assert(type(C.recency_in_weeks) == "number")
  assert(C.recency_in_weeks >= 0)

  assert(type(C.min_n_uq_lags) == "number")
  assert(C.min_n_uq_lags > 0)

  assert(type(C.min_n_knots) == "number")
  assert(C.min_n_knots > 0)

  assert(type(C.min_range_t_o_y) == "number")
  assert(C.min_range_t_o_y > 0)

  assert(type(C.min_n_uq_t_o_y) == "number")
  assert(C.min_n_uq_t_o_y > 0)

  assert(type(C.min_n_rows) == "number")
  assert(C.min_n_rows > 0)

  assert(type(C.start_lag) == "number")
  assert(C.start_lag >= 0)

  assert(type(C.stop_lag) == "number")
  assert(C.stop_lag >= 0)

  if ( C.start_lag == 0 ) then assert(C.stop_lag == 0) end 
  assert(C.start_lag <= C.stop_lag)
  assert(math.floor(C.start_lag) == math.ceil(C.start_lag))
  assert(math.floor(C.stop_lag) == math.ceil(C.stop_lag))

  -- START for Pass 3
  if ( C.pass3 ) then 
    assert(type(C.decay) == "number")
    assert(C.decay >= 0)
  
    assert(type(C.seasonality) == "number")
    assert(C.seasonality >= 0)
  
    assert(type(C.n_embeddings) == "number")
    assert(C.n_embeddings >= 0)
  
    assert(type(C.max_n_embeddings) == "number")
    assert(C.max_n_embeddings >= 0)
  
    assert(type(C.sh_hday_min_yrs) == "number")
    assert(C.sh_hday_min_yrs > 0)
  
    assert(type(C.sh_hday_min_n_per_yr) == "number")
    assert(C.sh_hday_min_n_per_yr > 0)
  
    assert(C.max_n_embeddings >= C.n_embeddings)
  
    assert(type(C.embed_prefix) == "string")
    assert(#C.embed_prefix > 0)
  
    assert(type(C.tcin_meta_file) == "string")
    assert(#C.tcin_meta_file > 0)
    print("Pass 3 checked")
  end
  -- STOP for Pass 3

  --== START for redo
  if ( C.redo ) then 
    local R = assert(C.redo) 
    assert(type(R) == "table")

    local failures = assert(R.failures)
    assert(type(failures) == "string")
    -- TODO P1 assert(plpath.exists(failures)) -- file must exist 

    local items_cross_styl = assert(R.items_cross_styl)
    assert(type(items_cross_styl) == "string")
    -- TODO P1 assert(plpath.exists(items_cross_styl)) -- file must exist 
  end

  --====== START for base week
  if (  C.str_base_week ) then 
    assert(type(C.str_base_week) == "string")
    assert(type(C.base_week_format) == "string")
    -- there are more checks but they are done in xread_coonfigs()
  end
  --====== STOP  for base week
  if ( C.max_stockout ) then 
    assert(type(C.max_stockout) == "number")
    assert(C.max_stockout > 0)
    assert(C.max_stockout <= 1)
  end

  --==  STOP for redo
  --== START for scoring 
  if ( C.default_formula ) then 
    assert(type(C.default_formula) == "string")
    assert(#C.default_formula > 0)
    -- There is checking happening in read_configs TODO P4
  end
  --== STOP  for scoring 
  ----== START: For error codes
  if ( C.err_codes ) then
    local E = C.err_codes
    assert(type(E) == "table")
    if ( E.plp1 ) then
      local P1 = E.plp1 
      assert(type(P1) == "table")
      local nP1 = 0
      for k, v in pairs(P1) do 
        nP1 = nP1 + 1 
        assert(type(v) == "string")
        assert(#v > 0)
      end 
      assert(nP1 > 0)
      for k, _ in pairs(P1) do 
        k = assert(tonumber(k))
        assert( (k >= 0 ) and ( k <= nP1-1 ) ) 
      end
    end
    if ( E.plp2 ) then
      local P2 = E.plp2 
      assert(type(P2) == "table")
      local nP2 = 0
      for k, v in pairs(P2) do 
        nP2 = nP2 + 1 
        assert(type(v) == "string")
        assert(#v > 0)
      end 
      assert(nP2 > 0)
      for k, _ in pairs(P2) do 
        k = assert(tonumber(k))
        assert( (k >= 0 ) and ( k <= nP2-1 ) ) 
      end
    end
  end
  ----== STOP : For error codes
  assert(type(C.major_version) == "number")
  assert(C.major_version >= 0)
  assert(type(C.minor_version) == "number")
  assert(C.minor_version >= 0)
  --===================================================
  -- START: for btcs
  if ( C.multiweek_event_n_weeks) then 
    assert(type(C.multiweek_event_n_weeks) == "number")
    assert(C.multiweek_event_n_weeks >= 0)
  end
  if ( C.multiweek_event_max_default_k ) then 
    assert(type(C.multiweek_event_max_default_k) == "number")
    assert(C.multiweek_event_max_default_k >= 0)
  end
  if ( C.btcs_K ) then 
    assert(type(C.btcs_K) == "number")
    if ( C.btcs_K < 0 ) then assert(C.btcs_K == -1) end 
  end
  if ( C.btcs_max_consecutive_ones ) then 
    assert(type(C.btcs_max_consecutive_ones) == "number")
    assert(C.btcs_max_consecutive_ones > 0)
  end
  -- STOP : for btcs
  --===================================================
  -- START for SHItemToyCmp  t_o_y in pass3/
  if ( C.toy_K ) then 
    assert(type(C.toy_K) == "number")
    if ( C.toy_K < 0 ) then assert(C.toy_K == -1) end 
  end
  if ( C.toy_max_default_K ) then 
    assert(type(C.toy_max_default_K) == "number")
    assert(C.toy_max_default_K > 0)
  end
  -- STOPT for SHItemToyCmp  t_o_y in pass3/
  assert(type(C.webserver_port) == "number")
  assert(C.webserver_port > 0)
  assert(C.webserver_port < 65535)
  return true
end
return chck_configs
--===========================================
