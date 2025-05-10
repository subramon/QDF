local ffi = require 'ffi'
local cutils = require 'libcutils'
local function str_qtypes_to_c_qtypes_widths(str_qtypes)
  assert(type(str_qtypes) == "table")
  local nC = #str_qtypes
  assert(nC >= 1)
  -- create an array of c_qtypes and c_widths 
  local l_qtypes = {}
  local l_widths = {}
  local c_widths = ffi.C.malloc(nC * ffi.sizeof("uint32_t"))
  c_widths = ffi.cast("uint32_t *", c_widths)
  local c_qtypes = ffi.C.malloc(nC * ffi.sizeof("qtype_t"))
  c_qtypes = ffi.cast("qtype_t *", c_qtypes)
  for k, v in ipairs(str_qtypes) do 
    c_widths[k-1] = cutils.get_width_qtype(v)
    c_qtypes[k-1] = cutils.get_c_qtype(v)
  end
  return c_qtypes, c_widths, nC
end
return  str_qtypes_to_c_qtypes_widths
