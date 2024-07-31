local ffi = require 'ffi'
local X = {}
X.I1 = ffi.sizeof("int8_t")
X.I2 = ffi.sizeof("int16_t")
X.I4 = ffi.sizeof("int32_t")
X.I8 = ffi.sizeof("int64_t")
X.UI1 = ffi.sizeof("uint8_t")
X.UI2 = ffi.sizeof("uint16_t")
X.UI4 = ffi.sizeof("uint32_t")
X.UI8 = ffi.sizeof("uint64_t")
X.F2 = ffi.sizeof("uint16_t")
X.F4 = ffi.sizeof("float")
X.F8 = ffi.sizeof("double")
X.TM = ffi.sizeof("tm_t") 
X.HL = ffi.sizeof("holiday_bmask_t") 
return X
