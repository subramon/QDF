local lQDF = require 'lQDF'
local C = require 'coalesce'
lQDF.q_register('coalesce', C)
assert(lQDF.q_is_register('coalesce'))
assert(not lQDF.q_is_spec('coalesce_F8_F8'))

-- for k, v in pairs(lQDF) do print(k, type(v)) end 

local x = lQDF({ 1, 2, 3, })
local y = lQDF({ 4, 5, 6, })
local z = lQDF.coalesce(x, y)
assert(lQDF.q_is_spec('coalesce_F8_F8'))
assert(type(z) == "lQDF")
lQDF.q_list()
lQDF.coalesce(x, y)
lQDF.q_dump()

print("All done")
