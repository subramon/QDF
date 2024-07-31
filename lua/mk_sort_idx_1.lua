-- Given a vector x, returns 
-- 1) srt_x = the sorted version of x and 
-- 2) srt_idx = the permutation to be applied to x to create srt_idx
-- Limitations: 
-- 1) works only for qtype(x) = I4
-- 2) works only for ascending sort order
local ffi                   = require 'ffi'
local lQDF                  = require 'lQDF'
--==============================================================
local function mk_sort_idx_1(x)
  assert(type(x) == "lQDF")
  assert(x:jtype() == "j_array")
  assert(x:qtype() == "I4") -- relax later
  local n  = x:num_elements()
  local sz = x:arr_size()

  -- make composite field with x in top 32 bits and index in bottom 32
  local tmp0 = x:convert("I8")
  local tmp1 = tmp0:shift_left(32)
  local tmp2  = lQDF.seq(0, 1, "I8", n, sz) -- an index field 
  local compfld = tmp1:bitwise_or(tmp2)
  -- sort composite field 
  compfld:sort("ascending")
  -- break apart composite field to get sorted x and sort order
  local tmp0 = compfld:shift_right(32)
  local srt_x = tmp0:convert("I4")

  local mask = (4*1024*1048576)-1
  local tmp1 = compfld:bitwise_and(mask)
  local srt_idx = tmp1:convert("I4")

  return srt_x, srt_idx
end
return mk_sort_idx_1
