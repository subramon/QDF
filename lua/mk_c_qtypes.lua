local ffi = require 'ffi'
local function mk_c_qtypes(qtypes)
  assert(type(qtypes) == "table")
  local ncols = #qtypes
  local sz = ncols * ffi.sizeof("void *")
  local c_qtypes = ffi.gc(ffi.C.malloc(sz), ffi.C.free)
  c_qtypes = ffi.cast("char **", c_qtypes)
  c_qtypes[0] = ffi.NULL
  for i, qtype in ipairs(qtypes) do
    assert(type(qtype == "string"))
    local len = #qtype + 1
    local x = ffi.gc(ffi.C.malloc(len), ffi.C.free)
    ffi.fill(x, len)
    ffi.copy(x, qtype, len)
    c_qtypes[i-1] = x
  end
  c_qtypes = ffi.cast("const char ** const ", c_qtypes)
  return c_qtypes
end
return mk_c_qtypes
