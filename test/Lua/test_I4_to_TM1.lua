local simdjson = require 'simdjson'
local lQDF = require 'lQDF'
local tests = {}
tests.t1 = function()

  local json_str = '[ 1, 1641100459, 1691120459, 1741120459, 1741130459]'
  local json = simdjson.parse(json_str)
  assert(type(json) == "table")
  local x = lQDF(json)
  assert(type(x) == "lQDF")
  assert(x:jtype() == "j_array")
  assert(x:qtype() == "F8")
  local y = x:convert("I4")
  assert(y:qtype() == "I4")

  local z = y:I4_to_TM1()
  assert(z:jtype() == "j_array")
  assert(z:qtype() == "TM1")

  y:pr_csv()
  z:pr_csv()
  

  print("Test t1 completed successfully")
end
tests.t1()
