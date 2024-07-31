G = {} -- to suprpess strict
G.debug= true
require 'strict'
local ffi = require 'ffi'
local cqtypes = require 'cqtypes'
local simdjson = require 'simdjson'
local lRBC = require 'lRBC'

local tests = {}
tests.t1  = function()
  local n = 10
  local start = 1
  local incr  = 1
  local qtypes = {"I8", }
  for _, qtype in ipairs(qtypes) do
    local str_json = '[ 1, 2, 3, 4, 2, 3, 4, 3, 4, 4 ] '
    local json = simdjson.parse(str_json)
    assert(type(json) == "table")
    local x = lRBC(json):convert(qtype)

    local y, z = x:vals_counts()
    assert(y:num_elements() == 4)
    assert(z:num_elements() == 4)
    assert(y:qtype() == x:qtype())
    assert(z:qtype() == "I4")
    assert(y:jtype() == "j_array")
    assert(z:jtype() == "j_array")
    local yctype = cqtypes[y:qtype()]
    local zctype = cqtypes[z:qtype()]
    print("yctype = ", yctype)
    print("zctype = ", zctype)
    local ny, yptr = y:get_arr_ptr()
    local nz, zptr = z:get_arr_ptr()
    assert(ny == nz)
    local yptr = ffi.cast(yctype .. " *", yptr)
    local zptr = ffi.cast(zctype .. " *", zptr)
    assert(yptr[0] == 1); assert(zptr[0] == 1); 
    assert(yptr[1] == 2); assert(zptr[1] == 2); 
    assert(yptr[2] == 3); assert(zptr[2] == 3); 
    assert(yptr[3] == 4); assert(zptr[3] == 4); 
    --=================================
  end
  print("Completed test t1 successfully")
end
tests.t1()
-- return tests
