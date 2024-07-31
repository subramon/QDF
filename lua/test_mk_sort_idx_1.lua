-- 2) works only for ascending sort order
local ffi           = require 'ffi'
local lQDF          = require 'lQDF'
local mk_sort_idx_1  = require 'mk_sort_idx_1'
--==============================================================
local function chk1(srt_x, srt_idx)
  assert(type(srt_x) == "lQDF")
  assert(srt_x:qtype() == "I4")
  assert(srt_x:jtype() == "j_array")
  
  assert(type(srt_idx) == "lQDF")
  assert(srt_idx:qtype() == "I4")
  assert(srt_idx:jtype() == "j_array")
  return true
end

local n = 16
  
local tests = {}
tests.t1 = function() 
  local x = lQDF.seq(1, 1, "I4", n)
  local srt_x, srt_idx = mk_sort_idx_1(x)
  assert(chk1(srt_x, srt_idx))

  local chk_srt_idx = lQDF.seq(0, 1, "I4", n)
  local n = chk_srt_idx:eq(srt_idx):sum()
  assert(n == x:num_elements())
  
  local chk_x = lQDF.seq(1, 1, "I4", n)
  local n = chk_x:eq(srt_x):sum()
  assert(n == x:num_elements())
  
  -- print(srt_x); print(srt_idx)
  print("Test t1 completed successfully")
end
tests.t2 = function() 
  local x = lQDF.seq(n*10, -10, "I4", n)
  local srt_x, srt_idx = mk_sort_idx_1(x)
  assert(chk1(srt_x, srt_idx))
  
  local chk_srt_idx = lQDF.seq(n-1, -1, "I4", n)
  local n = chk_srt_idx:eq(srt_idx):sum()
  assert(n == x:num_elements())
  
  local chk_x = lQDF.seq(10, 10, "I4", n)
  local n = chk_x:eq(srt_x):sum()
  assert(n == x:num_elements())
  
  -- print(srt_x); print(srt_idx)
  print("Test t2 completed successfully")
end
tests.t1()
tests.t2()
