local tests = {}
tests.t1 = require 'chk_tcin_map' -- gamm/
local status = tests.t1("pass3_tcin_map_1.rbc")
assert(status)
print("Tests completed successfully")
