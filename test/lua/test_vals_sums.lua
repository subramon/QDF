G = {} -- to suprpess strict
G.debug= true
require 'strict'
require 'strict'
local cqtypes = require 'cqtypes'
local ffi = require 'ffi'
local simdjson = require 'simdjson'
local lRBC = require 'lRBC'

local tests = {}
tests.t1  = function()
  local n = 10
  local start = 1
  local incr  = 1
  local qtypes = {"I8", }
  for _, qtype in ipairs(qtypes) do
    --=================================
    -- create input keys
    local str_json = '[ 1, 2, 3, 4, 2, 3, 4, 3, 4, 4 ] '
    local json = simdjson.parse(str_json)
    assert(type(json) == "table")
    local in_keys = lRBC(json):convert(qtype)
    -- create counts for corresponding entries 
    local str_json = '[ 1000, 200, 30, 4, 200, 30, 4, 30, 4, 4 ] '
    local json = simdjson.parse(str_json)
    assert(type(json) == "table")
    local in_nums = lRBC(json):convert("I4")
    print(in_keys)
    print(in_nums)

    local y, z = lRBC.vals_sums(in_keys, in_nums)
    print(y)
    print(z)
    --=================================
    assert(y:num_elements() == 4)
    assert(z:num_elements() == 4)
    assert(y:qtype() == in_keys:qtype())
    assert(z:qtype() == "I4")
    assert(y:jtype() == "j_array")
    assert(z:jtype() == "j_array")
    local yctype = cqtypes[y:qtype()]
    local zctype = cqtypes[z:qtype()]
    local ny, yptr = y:get_arr_ptr()
    local nz, zptr = z:get_arr_ptr()
    assert(ny == nz)
    local yptr = ffi.cast(yctype .. " *", yptr)
    local zptr = ffi.cast(zctype .. " *", zptr)
    assert(yptr[0] == 1); assert(zptr[0] == 1000); 
    assert(yptr[1] == 2); assert(zptr[1] == 400); 
    assert(yptr[2] == 3); assert(zptr[2] == 90); 
    assert(yptr[3] == 4); assert(zptr[3] == 16); 
    --=================================
  end
  print("Completed test t1 successfully")
end
tests.t1()
-- return tests
