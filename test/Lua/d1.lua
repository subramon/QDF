lRBC = require 'lRBC'
ffi  = require 'ffi'

col_names =  { "date1", "num1" }
qtypes    =  { "TM1:%Y-%m-%d", "F8" }
infile    = "../test/in2.csv"
optargs   = { is_hdr = true }

x = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
print(type(x))

y = x:get("date1")
print(y:jtype())
print(y:subtype())

n, tptr = y:get_arr_ptr()
print(n)
tptr = ffi.cast("tm_t *", tptr)
for i = 1, n do
  print(i, tptr[i-1].tm_yday, tptr[i-1].tm_year, tptr[i-1].tm_mon, tptr[i-1].tm_mday)
end

tb = y:time_band(4)
print(tb)

