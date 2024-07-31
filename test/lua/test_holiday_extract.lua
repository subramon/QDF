G = {}  --- to suppress strict 
G.debug= true
require 'strict'
require 'strict'
local ffi = require 'ffi'
local lRBC = require 'lRBC'

local tests = {}
tests.t1 = function()
  local x, y, z, w
  local vals = { 2, 6, 14, 30, 62 }
  local n = #vals
  x = lRBC.const(0, "I4", n)
  local chk_n, iptr = x:get_arr_ptr()
  assert(n == chk_n)
  iptr = ffi.cast("int32_t *", iptr)
  for k, v in ipairs(vals) do
    iptr[k-1] = v
  end
  assert(type(x) == "lRBC")
  assert(x:qtype() == "I4")
  y = lRBC.const(0, "I4", n)
  y:set_qtype("HL")
  x:set_qtype("HL")
  assert(y:qtype() == "HL")
  y:set(x)
  local H = { 
    "columbusday",
    "goodfriday",
    "easter",
    "fathersday_minus",
    "fathersday",
  }
  for i, h in ipairs(H) do 
    z = y:holiday_extract(h)
    assert(type(z) == "lRBC")
    assert(z:jtype() == "j_array")
    assert(z:qtype() == "I1")
    -- print(z:sum())
    assert(z:sum() == #H - (i-1))
  end
 
  print("Test t1 completed successfully")
end
tests.t1()
