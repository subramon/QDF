local ffi  = require 'ffi'
local lQDF = require 'lQDF'
local cQDF = ffi.load("libqdf.so")
local lqdfmem       = require 'lqdfmem'

local function foo(qdfs, l_keys)

  local n_qdfs = #qdfs
  local n_keys = #l_keys
  assert(n_keys > 0)
  assert(n_keys == n_qdfs)

  local c_keys = ffi.new("char *[?]", n_keys)
  for i, l_key in ipairs(l_keys) do 
    local len = #l_key + 1
    c_keys[i-1] = ffi.new("char[?]", len)
    ffi.fill(c_keys[i-1], len, 0)
    ffi.copy(c_keys[i-1], l_key)
  end
  local cqdf = lqdfmem(0)
  local cqdf_ptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
  local status = cQDF.make_mixed_array_or_object(ffi.NULL, 
    c_keys, n_keys, cqdf_ptr)
  assert(status == 0)
  local lqdf = setmetatable({}, lQDF)
  lqdf._cmem = cqdf
  if ( is_debug ) then assert(lqdf:check()) end
  return lqdf
end

local tests = {}
tests.t1 = function()
  local x = lQDF.binld("x.qdf")
  assert(x:check())
  assert(x:is_data_frame())
  local K = x:keys()
  -- for k, v in pairs(K) do print(k, v) end 
  -- local s = x:get("sls_unit_q")
  local v1 = x:get("size_per_unit_q")
  assert(v1:jtype() == "j_array")
  assert(v1:qtype() == "F4")

  local v2 = x:get("tcin")
  assert(v2:jtype() == "j_array")
  assert(v2:qtype() == "SC")
  assert(v2:width() == 16)  -- we know this from x.qdf

  local qdfs = { v1, v2 }
  local l_keys = { "size_per_unit_q", "tcin" }

  local out_qdf = foo(qdfs, l_keys)
  assert(type(out_qdf) == "lQDF")
  local K = out_qdf:keys()
  assert(#K == 2) 
  for k, v in pairs(K) do print(k, v) end 
  assert(out_qdf:check())

  print("Test t1 succeeded ")
end
tests.t1()
