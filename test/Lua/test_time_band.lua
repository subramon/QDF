G = {} -- to suppress strict 
G.debug= true
require 'strict'
require 'strict'
local lQDF = require 'lQDF'
local ffi  = require 'ffi'

local tests = {}
tests.t1 = function ()
  local col_names =  { "date1", "num1" }
  local qtypes    =  { "TM1:%Y-%m-%d", "F8" }
  local infile    = "../data/in2.csv"
  local optargs   = { is_hdr = true }

  local x = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))
  assert(type(x) == "lQDF")
  assert(x:jtype() == "j_object")
  -- print(x)

  local y = x:get("date1")
  assert(y:jtype() == "j_array")
  assert(y:qtype() == "TM1")

  local n, tptr = y:get_arr_ptr()
  tptr = ffi.cast("tm_t *", tptr)
  for i = 1, n do
    print(i, tptr[i-1].tm_yday, tptr[i-1].tm_year, tptr[i-1].tm_mon, tptr[i-1].tm_mday)
  end
  --=== time band 
  local recency = 4 
  local  tb = y:time_band(recency)
  assert(type(tb) == "lQDF")
  assert(tb:jtype() == "j_array")
  assert(tb:qtype() == "I1")
  print(tb)

  print("Successfully completed t1")
end
tests.t1()
