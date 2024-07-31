local plpath       = require 'pl.path'
local ffi          = require 'ffi'
local lQDF         = require 'lQDF'
local chck_configs = require 'chck_configs'
local file_to_str  = require 'file_to_str'
local json         = require 'json'
local stringify    = require 'stringify'
local trim_qtype   = require 'trim_qtype'
local holidays     = require 'holidays'
local reorder_formulas = require 'reorder_formulas'
-- check inputs as best as you can 
local chck_formulas              = require 'chck_formulas'
local chck_components            = require 'chck_components'
local chck_formula_specific_plp  = require 'chck_formula_specific_plp'
local chck_meta = require 'chck_meta'
local load_meta = require 'load_meta'
local mgcv_hdrs = require 'mgcv_hdrs'
assert(type(mgcv_hdrs) == "string")
local status = pcall(ffi.cdef, mgcv_hdrs)
if ( not status ) then print("mgcv_hdrs defined earlier") end 

-- assert(G) TODO P3 Do we need this?

local function trim(s) -- from PiL2 20.4
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end

local function chk_wip_data_set_file(wip_data_set_file)
  assert(type(wip_data_set_file) == "string")
  assert(#wip_data_set_file > 0)
  -- file names ends in ".lua"
  local n1, n2 = string.find(wip_data_set_file, ".lua")
  assert(n2 == #wip_data_set_file)
  assert((n1 + #(".lua") -1 ) == n2)
  return true
end

assert(type(holidays) == "table")
for k, v in pairs(holidays) do
  assert(type(v) == "string")
end
local widths       = require 'widths' -- after cdef of QDF_hdrs

local function set_width(n_cols, M)
  local x =  ffi.C.malloc(n_cols *  ffi.sizeof("int"))
  ffi.fill(x, n_cols *  ffi.sizeof("int"), 0)
  x =  ffi.cast("int *", x)
  for i =  1, n_cols do
    local w
    local qtype = assert(M[i].qtype)
    if ( qtype == "SV" ) then 
      -- TODO 
    elseif ( string.find(qtype, "SC:") == 1 ) then
      w = string.sub(qtype, 4, #qtype)
      w = assert(tonumber(w))
    else
      qtype = trim_qtype(qtype)
      w = assert(widths[qtype])
      assert(w > 0)
    end
    assert(w > 0)
    assert(type(w) == "number")
    x[i-1] = w
  end
  return x
end

local function read_configs(
  cC -- this is struct with C  configs
  )
  assert(type(G) == "table") -- this must exist in Lua state 
  assert(chck_configs(G.C))
  if ( G.components ) then 
    assert(chck_components(G.components, G.outM))
  end
  if ( G.F ) then 
    assert(chck_formulas(G.F))
  end
  if ( G.plp1 ) then assert(type(G.plp1) == "function") end 
  if ( G.plp2 ) then  assert(type(G.plp2) == "function") end
  chck_formula_specific_plp(G.F, G.P)
  assert(chck_meta(G.inM))
  assert(chck_meta(G.outM))
  assert(chck_meta(G.fwdM))
  local lC = assert(G.C) 
  -- we use lC for Lua version of configs and cC for C version

  cC = ffi.cast("config_t *", cC);
  assert(lC.cfg_dir)
  cC[0].cfg_dir  = stringify(lC.cfg_dir, "cfg_dir")

  --=============================================
  if ( G.Rrecv_selector ) then 
    assert(type(G.Rrecv_selector) == "table")

    assert(type(G.Rrecv_selector.fcast) == "boolean")
    cC[0].Rrecv_selector.fcast = G.Rrecv_selector.fcast

    assert(type(G.Rrecv_selector.model) == "boolean")
    cC[0].Rrecv_selector.model = G.Rrecv_selector.model
  end
  --=============================================
  if ( G.Rsend_selector ) then 
    assert(type(G.Rsend_selector) == "table")

    assert(type(G.Rsend_selector.hist) == "boolean")
    cC[0].Rsend_selector.hist = G.Rsend_selector.hist

    assert(type(G.Rsend_selector.fwd) == "boolean")
    cC[0].Rsend_selector.fwd = G.Rsend_selector.fwd

    assert(type(G.Rsend_selector.model) == "boolean")
    cC[0].Rsend_selector.model = G.Rsend_selector.model
  end
  --=============================================
  if ( G.load_selector ) then 
    assert(type(G.load_selector) == "table")

    assert(type(G.load_selector.hist) == "boolean")
    cC[0].load_selector.hist = G.load_selector.hist

    assert(type(G.load_selector.fwd) == "boolean")
    cC[0].load_selector.fwd = G.load_selector.fwd

    assert(type(G.load_selector.model) == "boolean")
    cC[0].load_selector.model = G.load_selector.model
  end
  --=============================================
  if ( G.thread_selector ) then 
    assert(type(G.thread_selector) == "table")

    assert(type(G.thread_selector.ds_reader) == "boolean")
    cC[0].thread_selector.ds_reader = G.thread_selector.ds_reader

    assert(type(G.thread_selector.prepper) == "boolean")
    cC[0].thread_selector.prepper = G.thread_selector.prepper

    assert(type(G.thread_selector.Rcaller) == "boolean")
    cC[0].thread_selector.Rcaller = G.thread_selector.Rcaller

    assert(type(G.thread_selector.Rwriter) == "boolean")
    cC[0].thread_selector.Rwriter = G.thread_selector.Rwriter

    assert(type(G.thread_selector.worker) == "boolean")
    cC[0].thread_selector.worker = G.thread_selector.worker

    assert(type(G.thread_selector.scorer) == "boolean")
    cC[0].thread_selector.scorer = G.thread_selector.scorer

    assert(type(G.thread_selector.model_writer) == "boolean")
    cC[0].thread_selector.model_writer = G.thread_selector.model_writer

    assert(type(G.thread_selector.fcast_writer) == "boolean")
    cC[0].thread_selector.fcast_writer = G.thread_selector.fcast_writer

    assert(type(G.thread_selector.webserver) == "boolean")
    cC[0].thread_selector.webserver = G.thread_selector.webserver
  end

  --=============================================
  local ddl_in = lC.ddl_in
  -- Note that it is okay to not specify any input files
  if ( ddl_in.data_set_files ) then 



    assert(type(ddl_in.data_set_files) == "table")
    local n_files  =  #ddl_in.data_set_files
    assert(n_files > 0)

    --=============================================
    assert(chk_wip_data_set_file(ddl_in.wip_data_set_file))

    local file_names, n_read, file_size, is_all_read

    file_names  =  ffi.C.malloc(n_files * ffi.sizeof("char *"))
    ffi.fill(file_names, (n_files * ffi.sizeof("char *")), 0)
    file_names  =  ffi.cast("char **", file_names)

    n_read  =  ffi.C.malloc(n_files * ffi.sizeof("uint64_t"))
    ffi.fill(n_read, (n_files * ffi.sizeof("uint64_t")), 0)
    n_read  =  ffi.cast("uint64_t *", n_read)

    is_all_read  =  ffi.C.malloc(n_files * ffi.sizeof("bool"))
    ffi.fill(is_all_read, (n_files * ffi.sizeof("bool")), 0)
    is_all_read  =  ffi.cast("bool *", is_all_read)

    file_size  =  ffi.C.malloc(n_files * ffi.sizeof("uint64_t"))
    ffi.fill(file_size, (n_files * ffi.sizeof("uint64_t")), 0)

    for i = 1, n_files do 
      local x = ddl_in.data_set_files[i]
      local l_file_name, l_n_read, l_is_all_read
      if ( type(x) == "string" )  then 
        l_file_name   = x
        l_n_read      = 0
        l_is_all_read = false
      elseif ( type(x) == "table") then 
        l_file_name   = x[1]
        l_n_read      = x[2]
        l_is_all_read = x[3]
      else
        error("ERROR")
      end
      assert(type(l_file_name) == "string")
      assert(#l_file_name > 0)
      -- TODO P0 assert(plpath.exists(l_file_name), "File not found " .. l_file_name)

      local x = string.format("file_names[%d]", i)
      file_names[i-1]  =  stringify(l_file_name, x)

      assert(type(l_n_read) == "number")
      assert(l_n_read >= 0)
      n_read[i-1] = l_n_read

      assert(type(l_is_all_read) == "boolean")
      is_all_read[i-1] = l_is_all_read
    end
    for i = 1, n_files-1 do 
      if ( is_all_read[i] == true ) then 
        assert(is_all_read[i-1] == true)
      end
    end

    cC[0].ddl_in.n_files     =  n_files
    cC[0].ddl_in.file_names  =  file_names
    cC[0].ddl_in.n_read      =  n_read
    cC[0].ddl_in.is_all_read =  is_all_read
    cC[0].ddl_in.file_size   =  file_size
    cC[0].ddl_in.wip_data_set_file = stringify(ddl_in.wip_data_set_file,
    "wip_data_set_file")
  end
  --====================================
  local n_cols, col_names, qtypes, is_load, has_nulls = 
    load_meta(lC.in_meta)
  if ( n_cols ) then 
    cC[0].in_meta.n_cols    = n_cols 
    cC[0].in_meta.col_names = col_names 
    cC[0].in_meta.qtypes    = qtypes 
    cC[0].in_meta.is_load   = is_load 
    cC[0].in_meta.has_nulls = has_nulls
    cC[0].in_meta.widths    = set_width(n_cols, lC.in_meta)
  end
  --===============================================
  local n_cols, col_names, qtypes, is_load, has_nulls = 
    load_meta(lC.fwd_meta)
  if ( n_cols ) then 
    cC[0].fwd_meta.n_cols    = n_cols 
    cC[0].fwd_meta.col_names = col_names 
    cC[0].fwd_meta.qtypes    = qtypes 
    cC[0].fwd_meta.is_load   = is_load 
    cC[0].fwd_meta.has_nulls = has_nulls
    cC[0].fwd_meta.widths    = set_width(n_cols, lC.fwd_meta)
  end
  --===============================================
  --[[
  for i =  1, n_cols do
    print(i, M[i].name, M[i].qtype)
  end
  --]]
 
  --====================================
  local Mout  =  assert(lC.out_meta)
  local num_regular_cols = #Mout
  local num_not_send_to_R = 0
  -- see which columns need an nn column
  local num_aux_cols = 0
  local Maux = {}
  for i, v in ipairs(Mout) do
    if ( v.is_load ~= nil ) then
      assert(type(v.is_load) == "boolean")
      if ( v.is_load == false ) then
        num_not_send_to_R = num_not_send_to_R + 1 
      end
    else
      v.is_load = true
    end
    if ( v.has_nulls ~= nil ) then
      assert(type(v.has_nulls) == "boolean")
      if ( v.has_nulls == true ) then 
        local t = { }
        t.name = "nn_" .. Mout[i].name
        t.qtype = "I1"
        Maux[#Maux+1] = t 
      end
    end 
  end
  -- print("XXX num_not_send_to_R = ", num_not_send_to_R )
  for i, v in ipairs(Mout) do
    assert(type(v.is_load) == "boolean")
  end
  local num_aux_cols = #Maux
  local n_cols = num_regular_cols + num_aux_cols 
  cC[0].Rmeta.n_cols = n_cols

  local col_names = ffi.C.malloc(n_cols * ffi.sizeof("char *"))
  ffi.fill(col_names, (n_cols * ffi.sizeof("char *")))
  cC[0].Rmeta.col_names = col_names

  local qtypes    = ffi.C.malloc(n_cols * ffi.sizeof("char *"))
  ffi.fill(qtypes, (n_cols * ffi.sizeof("char *")))
  cC[0].Rmeta.qtypes = qtypes

  local is_load   = ffi.C.malloc(n_cols * ffi.sizeof("bool"))
  ffi.fill(is_load, (n_cols * ffi.sizeof("bool")))
  cC[0].Rmeta.is_load = is_load

  local is_fcast   =  ffi.C.malloc(n_cols *  ffi.sizeof("bool"))
  ffi.fill(is_fcast, (n_cols *  ffi.sizeof("bool")), 0)
  cC[0].Rmeta.is_fcast = is_fcast
  
  local has_nulls   =  ffi.C.malloc(n_cols *  ffi.sizeof("bool"))
  ffi.fill(has_nulls, (n_cols *  ffi.sizeof("bool")), 0)
  cC[0].Rmeta.has_nulls = has_nulls
  
  --===========================
  --IMPORTANT: add Maux to Mout *after* setting the 
  -- data frame creation string.
  -- this is because the aux columns are not sent to R
  for i, v in ipairs(Maux) do
    Mout[#Mout+1] = v
  end
  local idx = 0
  for i, v in ipairs(Mout) do
    local x = string.format("M[%d].name", i)
    cC[0].Rmeta.col_names[idx] =  stringify(Mout[i].name, x)
    local x = string.format("M[%d].qtype", i)
    cC[0].Rmeta.qtypes[idx]    =  stringify(Mout[i].qtype, x)
    --=================================================
    if ( type(v.is_load) == "nil" ) then 
      cC[0].Rmeta.is_load[idx]   =  true  -- NOTE default assumption
    else
      assert(type(v.is_load) == "boolean") 
      cC[0].Rmeta.is_load[idx]   =  v.is_load
    end
    --=================================================
    if ( type(v.has_nulls) == "nil" ) then 
      cC[0].Rmeta.has_nulls[idx]   =  false  -- NOTE default assumption
    else
      assert(type(v.has_nulls) == "boolean") 
      cC[0].Rmeta.has_nulls[idx]   =  v.has_nulls
    end
    --=================================================
    if ( type(v.is_fcast) == "nil" ) then 
      cC[0].Rmeta.is_fcast[idx]   =  false  -- NOTE default assumption
    else
      assert(type(v.is_fcast) == "boolean") 
      cC[0].Rmeta.is_fcast[idx]   =  v.is_fcast
    end
    --=================================================

    idx  = idx + 1 
  end
  assert(idx == n_cols)
  cC[0].Rmeta.widths = set_width(n_cols, Mout)
  --== make R command to create a data frame from vectors
  -- The reason for the renv is because we are creating vectors in an
  -- environment called renv and hence data.frame must also be in that env
  local T = {}
  -- T[#T+1] = "renv$rsdf <- data.frame(" WIP 
  T[#T+1] = "rsdf <- data.frame("
  local first = true
  for i = 1, #Mout do
    if ( Mout[i].is_load ~= false ) then
      cC[0].Rmeta.is_load[i-1] = true
      if ( first == false ) then
        T[#T+1] = ", "
      else
        first = false
      end
      -- T[#T+1] = "renv$" .. Mout[i].name -- WIP
      T[#T+1] = Mout[i].name
    else
      cC[0].Rmeta.is_load[i-1] = false
    end
  end
  T[#T+1] = ");"
  local str = table.concat(T, "")
  cC[0].Rinfo.str_mk_df = stringify(str, "str_mk_df")
  --====================================
  local break_cols = assert(lC.ddl_in.data_set_identifier)
  local n_break_cols = #break_cols
  -- TODO P3 Fix Ugly hard code of 2, which should be MAX_NUM_BREAK_VALS
  assert(n_break_cols <= 2 ) 
  -- Note that the break_cols in C refers to the index of the column
  -- wheread the break_cols in Lua is the name of the of the column
  -- Hence, the following translation needs to be done 

  for i, c1 in ipairs(break_cols) do
    local break_col_idx = -1 
    for j, m in pairs(lC.in_meta) do 
      if ( m.name == c1 ) then
        break_col_idx = j
      end
    end
    assert(break_col_idx >= 0)
    cC[0].ddl_in.break_cols[i-1] = break_col_idx - 1 -- -1 for Lua to C 
  end

  local str_break_cols = ffi.C.malloc(n_break_cols *  ffi.sizeof("char *"))
  ffi.fill(str_break_cols, (n_break_cols *  ffi.sizeof("char *")))
  str_break_cols = ffi.cast("char **", str_break_cols)
  for i =  1, n_break_cols do
    local x = string.format("break_cols[%d]", i)
    str_break_cols[i-1] = stringify(break_cols[i], x)
  end

  cC[0].ddl_in.n_break_cols    = n_break_cols
  cC[0].ddl_in.str_break_cols  = str_break_cols
  --====================================
  cC[0].ddl_in.is_hdr   = lC.ddl_in.is_hdr
  cC[0].ddl_in.fld_sep  = lC.ddl_in.fld_sep
  cC[0].ddl_in.rec_sep  = lC.ddl_in.rec_sep
  cC[0].ddl_in.max_cell_width  = lC.ddl_in.max_cell_width
  cC[0].ddl_in.max_rows_in_df  = lC.ddl_in.max_rows_in_df

--[[
  for i =  1, for_R_n_cols do
    print(i, ffi.string(cC[0].for_R_col_names[i-1]), 
    ffi.string(cC[0].for_R_qtypes[i-1]), 
    tonumber(cC[0].for_R_widths[i-1]))
  end
--]]

  local Rinfo = assert(lC.rserve)
  if ( Rinfo.str_R_setup ) then
    local x = trim(Rinfo.str_R_setup)
    if ( #x > 0 ) then 
      cC[0].Rinfo.str_R_setup  = stringify(Rinfo.str_R_setup, "str_R_setup")
    end
  end

  cC[0].Rinfo.snd_timeout_sec = Rinfo.snd_timeout_sec
  cC[0].Rinfo.rcv_timeout_sec = Rinfo.rcv_timeout_sec
  cC[0].row_prod_qsz       = lC.row_prod_qsz;
  cC[0].ds_asm_qsz         = lC.ds_asm_qsz;
  cC[0].R_prep_qsz         = lC.R_prep_qsz;
  cC[0].R_call_qsz         = lC.R_call_qsz;
  cC[0].ds_prod_qsz        = lC.ds_prod_qsz;
  cC[0].R_work_qsz         = lC.R_work_qsz;
  cC[0].R_scor_qsz         = lC.R_scor_qsz;
  cC[0].data_set_expansion = lC.data_set_expansion;
  cC[0].max_num_workers    = lC.max_num_workers
  cC[0].max_num_Rcallers   = lC.max_num_Rcallers
  cC[0].max_num_scorers    = lC.max_num_scorers

  local servers = assert(Rinfo.servers)
  assert(type(servers) == "table")
  assert(#servers > 0)


  local max_num_servers = lC.rserve.max_num_servers
  local Rserve = ffi.C.malloc(max_num_servers * ffi.sizeof("Rserve_t"))
  ffi.fill(Rserve, (max_num_servers * ffi.sizeof("Rserve_t")))
  Rserve = ffi.cast("Rserve_t *", Rserve)

  for i, v in ipairs(servers) do 
    local x = string.format("server_address[%d]", i)
    Rserve[i-1].server = stringify(v.address, x)
    Rserve[i-1].port   = v.port
    Rserve[i-1].max_conn = v.max_conn
  end 
  cC[0].Rinfo.n_Rserve = #servers
  cC[0].Rinfo.sz_Rserve = max_num_servers
  cC[0].Rinfo.Rserve = Rserve
  -- we reorder formulas into an indexed array for 2 reasons
  -- (1) easier for C interactions
  -- (2) create dependencies and order of interactions
  local altF = assert(reorder_formulas(G.F))
  local nF  = #altF
  assert(nF > 0)
  cC[0].n_frmlas = nF
  --=====
  cC[0].dependence  = ffi.C.malloc(nF * ffi.sizeof("int"))
  ffi.fill(cC[0].dependence, (nF * ffi.sizeof("int")))

  cC[0].n_lags  = ffi.C.malloc(nF * ffi.sizeof("int"))
  ffi.fill(cC[0].n_lags, (nF * ffi.sizeof("int")))

  cC[0].lags  = ffi.C.malloc(nF * ffi.sizeof("int *"))
  ffi.fill(cC[0].lags, (nF * ffi.sizeof("int *")))

  cC[0].frmla_keys  = ffi.C.malloc(nF * ffi.sizeof("char *"))
  ffi.fill(cC[0].frmla_keys, (nF * ffi.sizeof("char *")))
  for k, v in ipairs(altF) do
    local x = string.format("frmla_keys[%d]", k)
    cC[0].frmla_keys[k-1]  = stringify(v.name, x)
    cC[0].dependence[k-1]  = v.parent - 1  -- -1 for Lua to C
    cC[0].n_lags[k-1]      = #v.lags
    -- print("num lags for " .. v .. " is " .. #G.F[v].lags)
    local n = #v.lags
    if ( n > 0 ) then 
      cC[0].lags[k-1]      = ffi.C.malloc(nF * ffi.sizeof("int"))
      cC[0].lags[k-1]      = ffi.cast("int *", cC[0].lags[k-1])
    end
    for j = 1, n do 
      cC[0].lags[k-1][j-1] = v.lags[j]
      -- print(j, " th lag for ", v, " is ", G.F[v].lags[j])
      -- lags must be provided in sorted order
      if ( j > 1 ) then 
        assert(v.lags[j-1] < v.lags[j] )
      end
    end

  end
  cC[0].default_frmla_idx = -1
  cC[0].default_frmla_key = stringify(lC.default_formula, "default_frmla_key")
  -- check that default formula key is a valid formula key
  for k, v in ipairs(altF) do
    if ( v.name == lC.default_formula ) then
      cC[0].default_frmla_idx = k - 1
      break
    end
  end
  assert((cC[0].default_frmla_idx >= 0), "Invalid default formula")
--=====================================================

  local nH = #holidays 
  assert(nH > 1)
  assert(nH <= 32) -- currently we assume holidays stored as 32-bit mask
  local sz = nH * ffi.sizeof("char *")
  local tmp = ffi.C.malloc(sz)
  ffi.fill(tmp, sz)
  tmp = ffi.cast("char **", tmp)
  for i, v in ipairs(holidays) do
    local x = string.format("holiday_lkp[%d]", i)
    tmp[i-1] = stringify(v, x)
  end
  cC[0].holiday_lkp = tmp
  cC[0].n_holiday_lkp = nH
  --====================================================
  cC[0].recency_in_weeks = lC.recency_in_weeks 
  cC[0].min_n_uq_lags = lC.min_n_uq_lags 
  cC[0].min_n_knots   = lC.min_n_knots   
  cC[0].min_range_t_o_y   = lC.min_range_t_o_y   
  cC[0].min_n_uq_t_o_y = lC.min_n_uq_t_o_y 
  cC[0].min_n_rows = lC.min_n_rows 
  cC[0].start_lag = lC.start_lag 
  cC[0].stop_lag = lC.stop_lag 
  cC[0].stop_lag = lC.stop_lag 
  -- START for Pass 2 extensions
  if ( lC.is_pass2ext ) then 
    cC[0].is_pass2ext = true
    cC[0].decay = lC.decay 
    cC[0].seasonality = lC.seasonality 
    cC[0].n_embeddings    = lC.n_embeddings
    cC[0].max_n_embeddings    = lC.max_n_embeddings
    cC[0].sh_hday_min_yrs    = lC.sh_hday_min_yrs
    cC[0].sh_hday_min_n_per_yr    = lC.sh_hday_min_n_per_yr
    cC[0].embed_prefix = stringify(lC.embed_prefix)
    cC[0].tcin_meta_file = stringify(lC.tcin_meta_file)
  end
  -- STOP for Pass 3

  cC[0].major_version = lC.major_version 
  cC[0].minor_version = lC.minor_version 

  -- START: for btcs 
  if ( lC.multiweek_event_n_weeks ) then 
    cC[0].multiweek_event_n_weeks = lC.multiweek_event_n_weeks
  end
  if ( lC.multiweek_event_max_default_k ) then 
    cC[0].multiweek_event_max_default_k = lC.multiweek_event_max_default_k
  end
  if ( lC.btcs_max_consecutive_ones ) then 
    cC[0].btcs_max_consecutive_ones = lC.btcs_max_consecutive_ones
  end
  if ( lC.btcs_K ) then 
    cC[0].btcs_K = lC.btcs_K 
   end
  -- STOP: for btcs 
  -- START  for SHItemToyCmp  t_o_y in pass3/
  if ( lC.toy_max_default_K ) then 
    cC[0].toy_max_default_K = lC.toy_max_default_K
  end
  if ( lC.toy_K ) then 
    cC[0].toy_K = lC.toy_K 
   end
  -- START  for SHItemToyCmp  t_o_y in pass3/

  -- START db_info 
  local db_info = assert(lC.save_info.db_info)
  assert(type(db_info) == "table")
  cC[0].db_info.docroot   = stringify(db_info.docroot, "docroot")
  -- STOP  db_info 
  --================================
  -- START disk_info   
  assert(type(lC.save_info.disk_info == "table"))
  local dir = lC.save_info.disk_info.input_dir 
  if ( dir ) then cC[0].disk_info.input_dir = stringify(dir, "input") end
  local dir = lC.save_info.disk_info.model_dir 
  if ( dir ) then cC[0].disk_info.model_dir = stringify(dir, "model") end
  local dir = lC.save_info.disk_info.fwd_dir 
  if ( dir ) then cC[0].disk_info.fwd_dir = stringify(dir, "fwd") end
  local dir = lC.save_info.disk_info.fcast_dir 
  if ( dir ) then cC[0].disk_info.fcast_dir = stringify(dir, "fcast") end
  local dir = lC.save_info.disk_info.incr_dir 
  if ( dir ) then cC[0].disk_info.incr_dir = stringify(dir, "incr") end
  -- STOP disk_info   
  --================================

  --=== START for week_start_date to holiday_bmask
  if ( lC.week_hol_file )  then 
    cC[0].week_hol_file = stringify(lC.week_hol_file, "week_hol_file")
    cC[0].week_hol_time_format = stringify(lC.week_hol_time_format,
    "week_hol_time_format") 
  end
  --=== STOP  for week_start_date to holiday_bmask

  -- START For redo
  if ( lC.redo and lC.redo.failures ) then 
    local F, IxS, IinS, SforI, ToDo = read_redo(lC)
    lC.ToDo = ToDo
    lC.IinS = IinS
    cC[0].n_redo = #ToDo
  end
  --  STOP For redo
  --====== START for base week
  if (  lC.str_base_week ) then 
    cC[0].str_base_week    = stringify(lC.str_base_week, "str_base_week")
    cC[0].base_week_format = stringify(lC.base_week_format, "base_week_format")
  end
  --====== STOP  for base week
  if ( lC.max_stockout ) then 
    cC[0].max_stockout = lC.max_stockout
  else
    cC[0].max_stockout = 0.75
  end
  --  START For components
  if ( G.components ) then 
    local nC = 0
    for k, v in pairs(G.components) do 
      nC = nC + 1 
    end
    cC[0].n_components = nC
    assert(#G.c_components == nC)
    --=========================================================
    local sz = nC * ffi.sizeof("component_t")
    local x = assert(ffi.C.malloc(sz))
    ffi.fill(x, sz)
    cC[0].components = x
    local idx = 0
    for _, comp_name in ipairs(G.c_components) do 
      assert(idx < nC)
      local v = assert(G.components[comp_name])
      cC[0].components[idx].name = stringify(comp_name)
      cC[0].components[idx].Rstr = stringify(v.Rstr)
      --+++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if ( v.class == 3 ) then 
        if ( v.feature_by) then 
          assert(type(v.feature_by) == "string")
          cC[0].components[idx].feature_by = stringify(v.feature_by)
        end
      end
      --+++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if ( v.class == 4 ) then
        assert(type(v.feature_name) == "string")
        cC[0].components[idx].feature_name = stringify(v.feature_name)
        assert(type(v.factor_name) == "string")
        cC[0].components[idx].factor_name = stringify(v.factor_name)
        --=======================================================
        local found = false
        for i, w in ipairs(Mout) do 
          if ( w.name == v.feature_name ) then 
            found = true; cC[0].components[idx].feature_idx = i-1
            break
          end
        end
        assert(found == true)
        --==================================================
        local found = false
        for i, w in ipairs(Mout) do 
          if ( w.name == v.factor_name ) then 
            found = true; cC[0].components[idx].factor_idx = i-1
            break
          end
        end
        assert(found == true)
      end
      --+++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if ( v.class == 5 ) then
        assert(type(v.feature_x) == "string")
        cC[0].components[idx].feature_x = stringify(v.feature_x)
        assert(type(v.feature_y) == "string")
        cC[0].components[idx].feature_y = stringify(v.feature_y)
        local found = false
        for i, w in ipairs(Mout) do 
          if ( w.name == v.feature_x ) then 
            found = true; cC[0].components[idx].feature_x_idx = i-1
            break
          end
        end
        assert(found == true)
        --==================================================
        local found = false
        for i, w in ipairs(Mout) do 
          if ( w.name == v.feature_y ) then 
            found = true; cC[0].components[idx].feature_y_idx = i-1
            break
          end
        end
        assert(found == true)
      end
      --=======================================================
      if ( v.class == 6 ) then
        assert(type(v.depends_on) == "table")
        local offset = assert(v.depends_on[1])
        assert(type(offset) == "string")
        local found = false
        for i, w in ipairs(Mout) do 
          if ( w.name == offset ) then 
            found = true; cC[0].components[idx].offset_idx = i-1
            break
          end
        end
        assert(found == true)
      end
        --==================================================
      cC[0].components[idx].Rstr = stringify(v.Rstr)
      cC[0].components[idx].model_type = v.class
      idx = idx + 1 
    end
    --=== START set depends_on
    local idx = 0
    for _,  comp_name in ipairs(G.c_components) do 
      -- print("Setting dependencies for " .. comp_name)
      local v = assert(G.components[comp_name])
      local D = v.depends_on
      assert(type(D) == "table")
      local nD = #D
      assert((nD >= 1 ) and (nD <= 2))  -- 2 could change in future 
      cC[0].components[idx].n_depends_on = nD
      local cD = ffi.C.malloc(nD * ffi.sizeof("int"))
      cD = ffi.cast("int *", cD); local didx = 0
      for _, w in ipairs(D) do 
        -- we now know that component v depends on column w in Mout
        local found = false
        for ridx, rcol in pairs(Mout) do 
          assert(type(rcol) == "table")
          if ( rcol.name == w ) then
            assert(didx < nD) 
            cD[didx] = ridx
            didx = didx + 1
            found  = true
            break
          end
        end
        assert(found == true)
      end 
      assert(idx < #G.c_components)
      cC[0].components[idx].depends_on = cD
      idx = idx + 1 
    end
    --=== STOP  set depends_on
  end
  --  STOP  For components
  --== START For error codes
  if ( lC.err_codes ) then
    local E = lC.err_codes
    if ( E.plp1 ) then
      local P1 = E.plp1 
      local nP1 = 0
      for k, v in pairs(P1) do 
        nP1 = nP1 + 1 
      end 
      cC[0].plp1_errs.n = nP1
      cC[0].plp1_errs.msgs = ffi.C.malloc(nP1 * ffi.sizeof("char *"))
      for i = 0, nP1-1 do 
        local x = string.format("plp1_errs.msgs[%d]", i)
        cC[0].plp1_errs.msgs[i] = stringify(P1[i], x)
      end
    end
    if ( E.plp2 ) then
      local P2 = E.plp2 
      local nP2 = 0
      for k, v in pairs(P2) do 
        nP2 = nP2 + 1 
      end 
      cC[0].plp2_errs.n = nP2
      cC[0].plp2_errs.msgs = ffi.C.malloc(nP2 * ffi.sizeof("char *"))
      for i = 0, nP2-1 do 
        local x = string.format("plp2_errs.msgs[%d]", i)
        cC[0].plp2_errs.msgs[i] = stringify(P2[i], x)
      end
    end
  end
  --== STOP  For error codes
  ----====================================================
  local rM = G.out_dfe
  if ( rM ) then
    local qtypes       = require 'qtypes'
    local N = ffi.C.malloc(#rM * ffi.sizeof("char *"))
    N = ffi.cast("char **", N)
    local S = ffi.C.malloc(#rM * ffi.sizeof("char *"))
    S = ffi.cast("char **", S)
    local Q = ffi.C.malloc(#rM * ffi.sizeof("qtype_t"))
    Q = ffi.cast("qtype_t *", Q)
    for i, v in ipairs(rM) do 
      N[i-1] = stringify(v.name)
      S[i-1] = stringify(v.qtype)
      Q[i-1] = qtypes[v.qtype]
    end
    cC[0].dfe_out_spec.name      = N
    cC[0].dfe_out_spec.str_qtype = S
    cC[0].dfe_out_spec.qtype     = Q
    cC[0].dfe_out_spec.n         =  #rM
  end
  ----====================================================
  cC[0].webserver_port = lC.webserver_port
  return true 
end
return read_configs 
