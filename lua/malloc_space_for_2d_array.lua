local  ffi = require 'ffi'
local function malloc_space_for_2d_array(nrows, ncols, widths)
  local sz = ncols * ffi.sizeof("void *")
  local out = ffi.gc(ffi.C.malloc(sz), ffi.C.free)
  ffi.fill(out, sz)
  out = ffi.cast("void **", out)
  for i = 1, ncols do 
    local width = widths[i]
    local sz = nrows * width
    local mem = assert(ffi.gc(ffi.C.malloc(sz), ffi.C.free))
    out[i-1] = mem
  end
  return out
end
return malloc_space_for_2d_array
