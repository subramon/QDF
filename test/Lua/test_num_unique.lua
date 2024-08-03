local simdjson = require 'simdjson'
local ffi = require 'ffi'
local lQDF = require 'lQDF'
local tests = {}

tests.num_unique = function()
  local b, x, n, y, z, w
  local str_json = '[ 10, 20, 30, 40, 50, 60, 10, 20, 30, 40, 50, 60]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  x = lQDF(json)
  assert(type(x) == "lQDF")
  n = x:num_unique()
  assert(n == 6)

  json_str = '[ 10, 20, 30, 40, 50, 60, 10, 20, 30, 40, 50, 60]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  x = lQDF(json)
  assert(type(x) == "lQDF")
  local b = x:is_unique()
  assert(b == false)
 
  local str_json = '[ 10, 10, 10, 10, 10 ]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  x = lQDF(json)
  assert(type(x) == "lQDF")
  local b = x:is_unique()
  assert(b == false)
 
  local str_json = '[ 10, 20, 30, 40, 50, 60, 10, 20, 30, 40, 50, 60]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  x = lQDF(json)
  assert(type(x) == "lQDF")
  for i = 1, 6 do 
    n = x:num_unique_saturated(i)
    assert(n == i)
  end

  print("Test num_unique completed successfully")
end

tests.num_unique()
