local ffi = require 'ffi'
local qtypes = require 'qtypes'
local function make_num_val( sclr, qtype)
  sclr = ffi.cast("QDF_SCLR_TYPE *", sclr)
  if ( qtype == qtypes.I1 ) then
    return tonumber(sclr[0].val.i1)
  elseif ( qtype == qtypes.I2 ) then
    return tonumber(sclr[0].val.i2)
  elseif ( qtype == qtypes.I4 ) then
    return tonumber(sclr[0].val.i4)
  elseif ( qtype == qtypes.I8 ) then
    return tonumber(sclr[0].val.i8)

  elseif ( qtype == qtypes.F2 ) then
    error("TODO") -- TODO P3
  elseif ( qtype == qtypes.F4 ) then
    return tonumber(sclr[0].val.f4)
  elseif ( qtype == qtypes.F8 ) then
    return tonumber(sclr[0].val.f8)

  elseif ( qtype == qtypes.UI1 ) then
    return tonumber(sclr[0].val.ui1)
  elseif ( qtype == qtypes.UI2 ) then
    return tonumber(sclr[0].val.ui2)
  elseif ( qtype == qtypes.UI4 ) then
    return tonumber(sclr[0].val.ui4)
  elseif ( qtype == qtypes.UI8 ) then
    return tonumber(sclr[0].val.ui8)
  else
    error("")
  end
end
return make_num_val
