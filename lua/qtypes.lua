local cutils = require 'libcutils'
local y = cutils.qtypes()
assert(type(y) == "string")
local z = loadstring(y)
assert(type(z) == "function")
local w = z()
assert(type(w) == "table")
-- for k, v in pairs(w) do print(k, v) end 
return w
