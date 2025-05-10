G = {} -- to avoid strict's complaints
G.debug= true
require 'strict'
local lQDF = require 'lQDF'

local tests = {}
tests.t1  = function(
  dstfile,
  srcfile,
  rsltsfile
  )
  -- read destination data frame
  local qtypes  =  { "I4", "I4", }
  local cols    =  { "idx", "key", }
  local infile  = dstfile
  local optargs = { is_hdr = true }
  print("READING ", infile)
  local M = {}
  for k, v in ipairs(cols) do
    M[k] = { name = cols[k], qtype = qtypes[k], }
  end
  local dst = assert(lQDF.read_csv(M, infile, optargs))
  local K = assert(dst:keys())
  assert(type(K) == "table")
  assert(dst:num_keys() == #cols)
  for i, k in ipairs(K) do 
    local x      = dst:get(k)
    assert(type(x) == "lQDF")
    assert(x:jtype() == "j_array")
    print("Verified " .. k)
  end
  -- extract fields of interest
  local dst_key      = dst:get("key")
  local dst_idx      = dst:get("idx")
  -- read source data frame
  local qtypes  =  { "I4", "I4", "I4", "I4", }
  local cols    =  { "idx", "key", "val","val_for_key", }
  local infile  = srcfile
  local optargs = { is_hdr = true }
  print("READING ", infile)
  local M = {}
  for k, v in ipairs(cols) do
    M[k] = { name = cols[k], qtype = qtypes[k], }
  end
  local src = assert(lQDF.read_csv(M, infile, optargs))

  -- extract fields of interest
  local src_key = src:get("key")
  local src_idx = src:get("idx")
  local src_val = src:get("val")
  -- do the join
  local dst_val, nn_dst_val = lQDF.left_join(
     src_key, src_idx, src_val, dst_key, dst_idx)
  -- read correct result
  local qtypes  =  { "I4", "I1",  }
  local cols    =  { "val", "nn_val", }
  local infile  = rsltsfile
  local optargs = { is_hdr = true }
  print("READING ", infile)
  local M = {}
  for k, v in ipairs(cols) do
    M[k] = { name = cols[k], qtype = qtypes[k], }
  end
  local rslt = assert(lQDF.read_csv(M, infile, optargs))
  -- compare rslt with dst_val
  local chk_val = rslt:get("val")
  assert(chk_val:num_elements() == dst_val:num_elements())
  assert(chk_val:qtype() == dst_val:qtype())
  assert(dst_val:eq(chk_val):sum() == dst_val:num_elements())
  print(dst_val)
  -- compare nn_rslt with nn_dst_val (if it exists 
  local chk_nn_val  = rslt:get("nn_val")
  if ( nn_dst_val ) then 
    print(nn_dst_val)
    assert(chk_nn_val:num_elements() == nn_dst_val:num_elements())
    assert(chk_nn_val:qtype() == nn_dst_val:qtype())
    assert(nn_dst_val:eq(chk_nn_val):sum() == nn_dst_val:num_elements())
  else -- all elements must be good 
    assert(chk_nn_val:sum() == dst_val:num_elements())
  end
  -- check dst_cal against manually curated one 

  print("Completed test t1 successfully")
end
tests.t1("data_join_dst_1.csv", "data_join_src_1.csv", 
  "data_join_rslt_1.csv")
tests.t1("data_join_dst_1.csv", "data_join_src_2.csv", 
  "data_join_rslt_2.csv")
-- os.exit()
-- return tests
