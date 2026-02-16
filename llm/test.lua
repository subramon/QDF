local lQDF = require 'lQDF'
local C = require 'coalesce'
lQDF.register('coalesce', C)

-- for k, v in pairs(lQDF) do print(k, type(v)) end 

local J = { 1, 2, 3, }
local x = lQDF(J)
print(x:qtype())
print(x:jtype())
print(x)
lQDF.coalesce(x, x)

lQDF.qfns.coalesce.test()
print("========")
for k, v in pairs(lQDF.qfns.coalesce) do print (k, type(v)) end 
print("++++++++")
for k, v in pairs(lQDF.qfns.coalesce.specializations.coalesce_F8_F8.subs) do print (k, v, type(v)) end 
print("========")

print("All done")
