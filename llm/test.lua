local lQDF = require 'lQDF'
local C = require 'coalesce'
lQDF.register('coalesce', C)

-- for k, v in pairs(lQDF) do print(k, type(v)) end 

local x = lQDF({ 1, 2, 3, })
local y = lQDF({ 4, 5, 6, })
local z = lQDF.coalesce(x, y)
assert(type(z) == "lQDF")
lQDF.q_list()
lQDF.coalesce(x, x)
lQDF.q_dump()

print("All done")
