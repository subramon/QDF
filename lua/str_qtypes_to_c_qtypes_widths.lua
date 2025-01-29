local function str_qtypes_to_c_qtypes_widths(str_qtypes)
  assert(type(str_qtypes) == "table")
  local nC = #str_qtypes
  assert(nC >= 1)
  -- create an array of c_qtypes and c_widths 
  local l_qtypes = {}
  local l_widths = {}
  local c_widths = ffi.C.malloc(nC * ffi.sizeof("uint32_t"))
  local c_qtypes = ffi.C.malloc(nC * ffi.sizeof("qtype_t"))
  for k, v in ipairs(qtypes) do 
    c_widths[k-1] = get_width_qtype(v)
    c_qtypes[k-1] = get_c_qtype(v)
  end
  assert(nC == nQ)
  return c_qtypes, c_qwidths, nC
end
return  str_qtypes_to_c_qtypes_widths
