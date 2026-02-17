local lQDF = require 'lQDF'
local x = lQDF({ 1, 2, 3, })
local y = lQDF({ 4, 5, 6, })

lQDF.q_restore()
lQDF.q_list()
-- error("PREMATURE")
local z = lQDF.coalesce(x, y)
assert(type(z) == "lQDF")

lQDF.coalesce(x, x)
print("All done")

