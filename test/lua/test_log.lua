G = {}
G.debug= true
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

tests.log = function ()
  local in_x = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100  } 
  local x = lRBC(in_x)
  assert(type(x) == "lRBC")
  assert(x:qtype() == "F8")
  assert(x:jtype() == "j_array")

  local y = x:log()
  assert(type(y) == "lRBC")
  assert(y:qtype() == "F8")
  assert(y:jtype() == "j_array")

  local str_y = tostring(y)
  assert(str_y == "[2.302585, 2.995732, 3.401197, 3.688879, 3.912023, 4.094345, 4.248495, 4.382027, 4.499810, 4.605170]")

  print("Test log completed successfully")
end
tests.log()
-- return tests
