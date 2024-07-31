local ffi = require 'ffi'
local function cast_ptr(ptr, qtype)
  if ( qtype == "I1" ) then
    return ffi.cast("int8_t *", ptr)
  elseif ( qtype == "I2" ) then
    return ffi.cast("int16_t *", ptr)
  elseif ( qtype == "I4" ) then
    return ffi.cast("int32_t *", ptr)
  elseif ( qtype == "I8" ) then
    return ffi.cast("int64_t *", ptr)
  elseif ( qtype == "F4" ) then
    return ffi.cast("float *", ptr)
  elseif ( qtype == "F8" ) then
    return ffi.cast("double *", ptr)
  else
    error("XXX")
  end
  return ffi.NULL
end
return cast_ptr
