local ffi = require 'ffi'
local lRBC = require 'lRBC'
local function demo_sum()
  local x = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
  local y = assert(lRBC(x))
  local n, dptr = y:get_arr_ptr()
  assert(n == #x)
  dptr = ffi.cast("double *", dptr)
  local sum = 0
  for  i = 1, n do
    sum = sum + dptr[i-1]
  end
  assert(sum == 45)
  local chksum = y:sum()
  assert(sum == chksum)
  print("Test demo_sum succeeded")
end
demo_sum()
