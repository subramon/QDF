G = {} -- to circumvent strict 
G.debug= true
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

tests.min_max = function ()
  local in_x = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100,  } 
  local x = lRBC(in_x)
  local in_y = { 100, 90, 80, 70, 60, 50, 40, 30, 20, 10, }
  local y = lRBC(in_y)
  print(x)
  print(y)
  --=================
  local z = x:max(y)
  print(z)
  local str_z = tostring(z)
  assert(str_z == "[100, 90, 80, 70, 60, 60, 70, 80, 90, 100]")
  --====================================
  local w = x:min(y)
  print(w)
  local str_w = tostring(w)
  assert(str_w == "[10, 20, 30, 40, 50, 50, 40, 30, 20, 10]")
  --====================================

  print("Test min_max completed successfully")
end
tests.min_max()
-- return tests
