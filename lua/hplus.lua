-- loads dataframe 1 from a file
-- determines which rows are to be deleted 
-- (based on week_start_date < xxx )
-- squeezes down df1 based on condition field set above
-- checks if enough space to append new data frame (df2)
-- if so, it does the append
-- else, it creates a new empty dataframe that is big enough
-- and then appends df1 and df2 to the empty data frame

local lQDF = require 'lQDF'
local time =  require 'posix.time'

local function hplus(
  lC, -- lua configs 
  oldfilename, -- used to create old df (df=data frame)
  incr_df -- incremental data frame to be added to old_df
  )
  -- NOTE: In production, we use the global G to access the Lua configs
  -- NOTE: In testing, we use the parameter lC to access the Lua configs
  if ( not lC ) then assert(type(G) == "table"); lC = G.lC end 

  assert(type(lC) == "table")
  assert(type(oldfilename) == "string")
  assert(type(incr_df) == "lQDF")
  local is_df, n, sz = incr_df:is_data_frame()
  -- print("INCR n/sz = ", n, sz)

  local D = {} -- debugging info returned to caller

  assert(type(lC) == "table")
  assert(type(lC.hplus) == "table")
  local time_fld = assert(lC.hplus.time_fld) -- e.g,  week_start_date
  local cutoff_date    = assert(lC.hplus.cutoff_date)
  -- minimum value of time below which rows will be eliminated
  local format   = assert(lC.hplus.format) -- e.g., "%Y-%m-%d
  --==================================
  local writeable = true 
  local old_df = lQDF.binld(oldfilename, writeable)
  assert(type(old_df) == "lQDF")
  assert(old_df:check())
  local is_df, n, sz = old_df:is_data_frame()
  -- print("ORIG n/sz = ", n, sz)
  assert(is_df == true)
  assert(n > 0)
  D.sz_old_df = sz
  D.n_old_df  = n
  local T = time.strptime(cutoff_date, format)
  local tsec = time.mktime(T)
  D.tsec = tsec

  local tm_fld = old_df:get(time_fld)
  assert(type(tm_fld) == "lQDF")
  assert(tm_fld:qtype() == "TM")
  
  local t_epoch = tm_fld:mktime()
  assert(type(t_epoch) == "lQDF")
  assert(t_epoch:qtype() == "I4")

  local cfld = t_epoch:vs_op(">", tsec, "I4")
  assert(type(cfld) == "lQDF")
  assert(cfld:qtype() == "I1")
  D.num_to_keep = cfld:sum()

  -- IMPORTANT: To make this opeation idempontent, we enforce:
  -- minimum time of incr_df > maximum time of old_df OR 
  -- maximum time of incr_df < minimum time of old_df OR 

  -- min/max of old 
  local min_old = t_epoch:min()
  local max_old = t_epoch:max()
  -- min/max of incr
  local incr_tm_fld = incr_df:get(time_fld)
  local incr_t_epoch = incr_tm_fld:mktime()
  local min_incr = incr_t_epoch:min()
  local max_incr = incr_t_epoch:max()

  -- Constraint: the time periods of the two dataframes 
  -- (original and incremental) cannot overlap 
  local orig_file = "data_hplus_orig_1.csv"
  if ( not ( ( min_incr > max_old ) or ( max_incr < min_old ) )  ) then
    return 1; -- error code 
  end

  --=====================================
  local num_to_expand = 0

  old_df:squeeze_where(cfld)
  local post_is_df, post_n, post_oldsz = old_df:is_data_frame()
  -- print("OLD  n/sz = ", post_n, post_oldsz)
  assert(post_is_df == true)
  D.post_n_old_df = post_n
  D.post_sz_old_df = post_oldsz 
  local old_keys = old_df:keys()
  local new_keys = incr_df:keys()
  local ret = old_df:append_df(incr_df)
  local post_is_df, post_n, post_oldsz = old_df:is_data_frame()
  -- print("POST n/sz = ", post_n, post_oldsz)
  -- TODO P1 What should we be doing with return from append_df?
  if ( type(ret) == "number") then
    num_to_expand = ret
    assert(num_to_expand > 0)
  else
    -- print("no need to expand")
    -- old_df:pr_df_as_csv(keys, "_after.csv")
    return D
  end
  -- need to make space 
  D.num_to_expand = num_to_expand
  local new_n = sz + num_to_expand -- note, NOT n + num_to_expand
  local new_sz = 1
  while new_sz < new_n do
    new_sz = new_sz * 2
  end
  D.new_n  = new_n
  D.new_sz = new_sz
  local keys = old_df:keys()
  local qtypes = {}
  for _, key in ipairs(keys) do 
    qtypes[#qtypes+1] = old_df:get(key):qtype()
  end
  local new_df = lQDF.make_empty_data_frame(keys, qtypes, new_sz)
  assert(new_df:append_df(old_df, false))
  assert(new_df:append_df(incr_df, false))
  assert(new_df:bindmp(oldfilename))
  return D
end
return hplus
