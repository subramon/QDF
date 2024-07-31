G = {} 
G.debug= true
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

tests.t1 = function ()
  local in_x = { 1.10, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 1.100,  } 
  local x = lRBC(in_x)
  assert(type(x) == "lRBC")
  assert(x:jtype() == "j_array")
  assert(x:qtype() == "F8")

  local y = x:round()

  assert(type(y) == "lRBC")
  assert(y:jtype() == "j_array")
  assert(y:qtype() == "F8")

  local str_y = tostring(y)
  print(str_y)
  assert(str_y == "[1, 1, 1, 1, 2, 2, 2, 2, 2, 1]")
  --====================================
  print("Test t1 completed successfully")
end
tests.t1()
-- return tests
