local ffi    = require 'ffi'
local widths = require 'widths'
local trim_qtype = require 'trim_qtype'

local function alloc_csv_out(qtypes, nrows, ncols)
  local sz = ncols * ffi.sizeof("void *")
  local out = ffi.gc(ffi.C.malloc(sz), ffi.C.free)
  ffi.fill(out, sz)
  out = ffi.cast("void **", out)
  for i = 1, ncols do 
    local qtype = trim_qtype(qtypes[i])
    local width = assert(widths[qtype], "Invalid qtype = " .. qtype)
    local sz = nrows * width
    local x = ffi.gc(ffi.C.malloc(sz), ffi.C.free)
    ffi.fill(x, sz)
    out[i-1] = x
  end
  return out
end
return alloc_csv_out
