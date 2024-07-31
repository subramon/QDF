ffi = require 'ffi'
lRBC = require 'lRBC'

x = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
y = lRBC(x)
n, dptr = y:get_arr_ptr()
dptr = ffi.cast("double *", dptr)
sum = 0
for  i = 1, n do
  sum = sum + dptr[i-1]
end

chksum = y:sum()
