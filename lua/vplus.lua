local debug = true 
local lQDF = require 'lQDF'

local function mk_composite_fld(tmfld, locfld)
  local time_in_sec = tmfld:mktime()
  local time_in_day = time_in_sec:vs_op("/", 24*60*60, "I4")
  local n = time_in_sec:num_elements()
  local sz = time_in_sec:size()
  local compfld = lQDF.const(0, "I8", n, sz)
  local idxfld = lQDF.seq(0, 1, "I4", n, sz)
  assert(compfld:shift_op(locfld, "or", 48))
  assert(compfld:shift_op(time_in_day, "or",  32))
  assert(compfld:shift_op(idxfld, "or", 0))
  compfld:sort("ascending")

  local key = compfld:shift_mask(32, "right", 4294967295, "I4")
  assert(type(key) == "lQDF")

  local idx = compfld:shift_mask(0, "right", 4294967295, "I4")
  assert(type(idx) == "lQDF")

  return key, idx
end
assert(type(mk_composite_fld) == "function")

local function vplus(
  lC, -- lua configs 
  oldfilename, -- used to create old df (df=data frame)
  incr_df, -- incremental data frame to be added to old_df
  newfilename -- can be same as oldfilename 
  )
  assert(type(lC) == "table")
  assert(type(oldfilename) == "string")
  if ( newfilename == nil ) then
    newfilename =  oldfilename
  end
  assert(type(incr_df) == "lQDF")

  local D = {} -- debugging info returned to caller

  --=========================
  -- NOTE: In production, we use the global G to access the Lua configs
  -- NOTE: In testing, we use the parameter lC to access the Lua configs
  if ( not lC ) then assert(type(G) == "table"); lC = G.lC end 
  assert(type(lC) == "table")
  --=========================
  local join_cols = assert(lC.vplus.join_cols)
  assert(type(join_cols) == "table")
  assert(#join_cols > 0)
  assert(#join_cols == 2) -- TODO P3 hard coded for now 

  local xfer_cols = assert(lC.vplus.xfer_cols)
  assert(type(xfer_cols) == "table")
  assert(#xfer_cols > 0) -- should be at least one column to transfer
  --==================================
  local writeable = false
  local old_df = lQDF.binld(oldfilename, writeable)
  assert(type(old_df) == "lQDF")
  if ( debug ) then 
    assert(old_df:check())
  end
  local is_df, n, sz = old_df:is_data_frame()
  assert(is_df == true); assert(n > 0); assert(n <= sz)
  --==================================
  -- columns to be transferred must not exist in destination
  for _, col in pairs(xfer_cols) do
    assert(old_df:is_key(col) == false)
  end
  --==================================
  -- TODO P3 Assumption:  first join col is TM and second is I2
  local tmfld = join_cols[1]
  local i2fld = join_cols[2]
  local from_tm = assert(incr_df:get(tmfld));assert(from_tm:qtype() == "TM")
  local from_i2 = assert(incr_df:get(i2fld));assert(from_i2:qtype() == "I2")

  local to_tm = assert(old_df:get(tmfld)); assert(to_tm:qtype() == "TM")
  local to_i2 = assert(old_df:get(i2fld)); assert(to_i2:qtype() == "I2")

  local from_key, from_idx = mk_composite_fld(from_tm, from_i2)
  local to_key,   to_idx   = mk_composite_fld(to_tm,   to_i2)

  -- I4 is limitation of current implementation
  assert(from_key:jtype() == "j_array"); assert(from_key:qtype() == "I4")
  assert(from_idx:jtype() == "j_array"); assert(from_idx:qtype() == "I4")

  local newflds = {}
  local nn_newflds = {}
  for k, v in ipairs(xfer_cols) do 
    local from_val = incr_df:get(v)
    local x, y = lQDF.left_join(
      from_key, from_idx, from_val, to_key, to_idx)
    assert(x)
    newflds[k] = x
    nn_newflds[k] = y
  end
  -- TODO P4 I am changing the meaning of lC at this point - ugly
  local lC = {} -- new columns
  local lQ = {} -- new qtypes 
  local lR = {} -- new QDFs
  -- first put in the old columns 
  local old_keys = old_df:keys()
  for k, v in ipairs(old_keys) do 
    lC[#lC+1] = v
    lQ[#lQ+1] = old_df:get(v):qtype()
    lR[#lR+1] = old_df:get(v)
  end
  -- then put in the columns transferred because of vplus 
  for k, v in ipairs(xfer_cols) do
  -- now we need to combine newly created fields and old data frame
  -- into a new data frame
    assert(newflds[k])
    lC[#lC+1] = v
    lQ[#lQ+1] = newflds[k]:qtype()
    lR[#lR+1] = newflds[k]
    --===========================
    if ( nn_newflds[k] ) then 
      lC[#lC+1] = "nn_" .. v
      lQ[#lQ+1] = "I1"
      lR[#lR+1] = nn_newflds[k]
    end
  end
  local _, _, sz = old_df:is_data_frame()
  local mtdf = lQDF.make_empty_data_frame(lC, lQ, sz)
  assert(mtdf)
  -- populate empty data frame created above
  for k, v in ipairs(lC) do
    mtdf:place_in_data_frame(v, lR[k])
  end
  mtdf:bindmp(newfilename)
  --==============
  return mtdf
end
return vplus
