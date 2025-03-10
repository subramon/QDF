local ffi = require 'ffi'
local qtypes = require 'qtypes'
local function lua_num_as_sclr(num, qtype)
  assert(type(num) == "number")
  if ( qtype == nil ) then qtype = "F8" end
  assert(type(qtype) == "string")
  --=========================================
  local sclr = ffi.new("SCLR_REC_TYPE[?]", 1)
  sclr = ffi.cast("SCLR_REC_TYPE *", sclr)
  sclr[0].qtype = assert(qtypes[qtype])

  if ( qtype == "I1" ) then 
    sclr[0].val.i1 = num
  elseif ( qtype == "I2" ) then 
    sclr[0].val.i2 = num
  elseif ( qtype == "I4" ) then 
    sclr[0].val.i4 = num
  elseif ( qtype == "I8" ) then 
    sclr[0].val.i8 = num
  elseif ( qtype == "UI1" ) then 
    sclr[0].val.ui1 = num
  elseif ( qtype == "UI2" ) then 
    sclr[0].val.ui2 = num
  elseif ( qtype == "UI4" ) then 
    sclr[0].val.ui4 = num
  elseif ( qtype == "UI8" ) then 
    sclr[0].val.ui8 = num
  elseif ( qtype == "F4" ) then 
    sclr[0].val.f4 = num
  elseif ( qtype == "F8" ) then 
    sclr[0].val.f8 = num
  else
    print("ERROR in lua_num_as_sclr")
    return nil
  end
  return sclr
end
return lua_num_as_sclr
