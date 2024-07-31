G = {} -- to supprss strict
G.debug= true
require 'strict'
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

tests.clamp = function ()
  local in_x = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100  } 
  local x = lRBC(in_x)
  assert(type(x) == "lRBC")
  assert(x:qtype() == "F8")
  print(x)

  local y = x:max(70)
  local n1 = y:max()
  print(y)
  assert(n1 == 70)

  local z = x:min(30)
  local n1 = z:min()
  assert(n1 == 30)

  print(z)

  print("Test clamp completed successfully")
end
tests.clamp()
-- return tests
