local ffi = require 'ffi'
local simdjson = require 'simdjson'
local x = [[
{ 
  "x0" : null,
  "x1" :  true, 
  "x2" :  false,
  "x3" :  1.2345,
  "x4" :  12345,
  "x5" : [],
  "x5" : [ "abc", "def" ],
  "x6" : {},
  "x7" : { "abc" : "def", "xyz" : 123 }
}
]]
local T = simdjson.parse(x)
print(type(T))
for k, v in pairs(T) do 
  if ( k == "x0" ) then 
    print(v, type(v))
    local x = ffi.cast("void *", v)
    print("x = ", x)
    assert(x == ffi.NULL)
  end
  if ( type(v) == "table" ) then
    for k2, v2 in pairs(v) do
      print(k, k2, v2) 
    end 
  else
    print(k, v)
  end
end
print("All done")

