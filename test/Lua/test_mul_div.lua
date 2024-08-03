G = {} -- to circumvent strict 
G.debug= true
require 'strict'
local lQDF = require 'lQDF'
local tests = {}

tests.mul_div = function ()
  local in_x = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100,  } 
  local x = lQDF(in_x)
  local in_y = { 100, 90, 80, 70, 60, 50, 40, 30, 20, 10, }
  local y = lQDF(in_y)
  print(x)
  print(y)
  --=================
  local z = x:div(y)
  print(z)
  local str_z = tostring(z)
  assert(str_z == "[0.100000, 0.222222, 0.375000, 0.571429, 0.833333, 1.200000, 1.750000, 2.666667, 4.500000, 10]")
  --====================================
  local w = x:mul(y)
  print(w)
  local str_w = tostring(w)
  assert(str_w == "[1000, 1800, 2400, 2800, 3000, 3000, 2800, 2400, 1800, 1000]")
  --====================================

  print("Test mul_div completed successfully")
end
tests.mul_div()
-- return tests
