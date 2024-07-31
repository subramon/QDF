G = {} 
G.debug= true
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

tests.t1 = function ()
  local in_x = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100,  } 
  local x = lRBC(in_x)
  assert(type(x) == "lRBC")
  assert(x:jtype() == "j_array")
  assert(x:qtype() == "F8")

  local y = x:convert("F4")

  assert(type(y) == "lRBC")
  assert(y:jtype() == "j_array")
  assert(y:qtype() == "F4")

  local str_y = tostring(y)
  assert(str_y == "[10, 20, 30, 40, 50, 60, 70, 80, 90, 100]")

  local z = y:convert("I4")

  assert(type(z) == "lRBC")
  assert(z:jtype() == "j_array")
  assert(z:qtype() == "I4")

  local str_z = tostring(z)
  assert(str_z == "[10, 20, 30, 40, 50, 60, 70, 80, 90, 100]")

  local w = z:convert("F4")

  assert(type(w) == "lRBC")
  assert(w:jtype() == "j_array")
  assert(w:qtype() == "F4")

  local str_w = tostring(w)
  assert(str_w == "[10, 20, 30, 40, 50, 60, 70, 80, 90, 100]")


  --====================================
  print("Test t1 completed successfully")
end
tests.t1()
-- return tests
