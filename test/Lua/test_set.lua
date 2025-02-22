local simdjson = require 'simdjson'
local ffi = require 'ffi'
local lQDF = require 'lQDF'
local tests = {}

tests.set = function()
  local x, y, z, w
  -- create x
  local str_json = '[ 10, 20, 30, 40, 50, 60, 10, 20, 30, 40, 50, 60]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local x = lQDF(json)
  assert(type(x) == "lQDF")
  print(x)

  -- create y
  local str_json = '[ 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local y = lQDF(json)
  assert(type(y) == "lQDF")

  x:set(y)
  print(x)
  assert(x:check())
  
 
  print("Test set completed successfully")
end

tests.set()
