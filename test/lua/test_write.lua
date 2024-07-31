G = {} -- to suppress strict
G.debug= true
require 'strict'
require 'strict'
local simdjson = require 'simdjson'
local lRBC = require 'lRBC'
local tests = {}

tests.test1 = function ()
  -- create x
  local str_json = '[ 10, 20, 30, 40, 50, 60]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local x = lRBC(json)
  assert(type(x) == "lRBC")

  -- create y
  local str_json = '[ 1, 2, 3, 4, 5, 6]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local y = lRBC(json)
  assert(type(y) == "lRBC")

  assert(x:write(y, 0))
  local str = lRBC.pr(x)
  assert(str == "[1, 2, 3, 4, 5, 6]")

  -- create x
  local str_json = '[ 10, 20, 30, 40, 50, 60]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local x = lRBC(json)
  assert(type(x) == "lRBC")

  -- create y
  local str_json = '[ 1, 2, 3, 4, 5]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local y = lRBC(json)
  assert(type(y) == "lRBC")

  assert(x:write(y, 1))
  local str = lRBC.pr(x)
  assert(str == "[10, 1, 2, 3, 4, 5]")

  -- create x
  local str_json = '[ 10, 20, 30, 40, 50, 60]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local x = lRBC(json)
  assert(type(x) == "lRBC")

  -- create y
  local str_json = '[ 1, 2, 3, 4, 5, 6]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local y = lRBC(json)
  assert(type(y) == "lRBC")

  print(">>> DELIBERATE ERROR")
  local status, x = pcall(lRBC.write, x, y, 1)
  print("<<< DELIBERATE ERROR")
  assert(not status)

  local str_json = '[ 10, 20, 30, 40, 50, 60]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local x = lRBC(json)
  assert(type(x) == "lRBC")

  local str_json = '[ 1, 2, 3, 4, 5, 6]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  local y = lRBC(json)
  assert(type(y) == "lRBC")

  assert(x:write(y, 1, true))
  local str = lRBC.pr(x)
  assert(str == "[10, 1, 2, 3, 4, 5]")

  print("Test test1 completed successfully")
end
tests.test1()
-- return tests
