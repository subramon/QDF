local ffi  = require 'ffi'
-- TODO P2 Do we need this? local cmem = require 'libcmem'

local function get_qdf_rec(w)
  if ( not w ) then print("no input") return nil end 
  if ( not  w._cmem )then print("no data in input") return nil end 
  local x = assert(w._cmem)
  local y = ffi.cast("QDF_REC_TYPE *", x)
  if ( y[0].data == ffi.NULL) then print("no data in cmem"); return nil end
  if ( y[0].size == 0 ) then print("zero size data in cmem"); return nil end
  return y
end
return get_qdf_rec
