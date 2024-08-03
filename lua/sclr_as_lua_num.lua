local ffi = require 'ffi'
local qtypes = require 'qtypes'
local function sclr_as_lua_num(x)
  assert(x)
  x =  ffi.cast("SCLR_REC_TYPE * ", x)
  if ( x[0].qtype == qtypes.I1 ) then
    return tonumber(x[0].val.i1)
  elseif ( x[0].qtype == qtypes.UI1 ) then
    return tonumber(x[0].val.ui1)
  elseif ( x[0].qtype == qtypes.I2 ) then
    return tonumber(x[0].val.i2)
  elseif ( x[0].qtype == qtypes.UI2 ) then
    return tonumber(x[0].val.ui2)
  elseif ( x[0].qtype == qtypes.I4 ) then
    return tonumber(x[0].val.i4)
  elseif ( x[0].qtype == qtypes.UI4 ) then
    return tonumber(x[0].val.ui4)
  elseif ( x[0].qtype == qtypes.I8 ) then
    return tonumber(x[0].val.i8)
  elseif ( x[0].qtype == qtypes.UI8 ) then
    return tonumber(x[0].val.ui8)
  elseif ( x[0].qtype == qtypes.F2) then
    error("not implemented") -- TODO P3
  elseif ( x[0].qtype == qtypes.F4) then
    return tonumber(x[0].val.f4)
  elseif ( x[0].qtype == qtypes.F8 ) then
    return tonumber(x[0].val.f8)
  else
    print("bad sclr_as_lua_num ")
    return nil
  end
end
return sclr_as_lua_num
