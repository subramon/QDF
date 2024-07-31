local lRBC = require 'lRBC'
local simdjson = require 'simdjson'

local tests = {}
tests.t1 = function()
  
  local x = lRBC({  0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0 }):convert("I1")
  local y = lRBC({  0, 0, 0, 1, 2, 3, 0, 0, 1, 2, 3, 4, 0 }):convert("I1")
  local z = x:sawtooth()
  assert(z:is_sawtooth())
  assert(y:neq(z):sum() == 0)

  local w = z:flatten_sawtooth()
  local v = lRBC({  0, 0, 0, 3, 3, 3, 0, 0, 4, 4, 4, 4, 0 }):convert("I1")
  assert(w:neq(v):sum() == 0) --> w == v
  
  print("Test t1 completed successfully")
end
tests.t1()
