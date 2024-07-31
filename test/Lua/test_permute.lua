G = {} -- to bypass strict 
G.debug= true
local plfile = require 'pl.file'
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

--===============================
tests.t1 = function ()
  local n = 10
  local idx = lRBC.seq(0, 1, "I4", n)
  local rev_idx = lRBC.seq(n-1, -1, "I4", n)
  local val =  lRBC.seq(10, 10, "I4", n)
  local p1 = val:permute(idx)
  print("rev_idx")
  print(rev_idx)
  print("values")
  print(val)
  local p2 = val:permute(rev_idx)
  print(p2)
  local p3 = p2:permute(rev_idx)
  local p3 = p2:permute(rev_idx)
  print(p3)
  assert(p3:neq(val):sum() == 0)
  print("Test t1 completed successfully")
end
tests.t1 = function ()
  local x = lRBC({5, 6, 4, 7, 3, 8, 2, 9, 1, 10, 0})
  x = x:convert("I4")
  local y = x:permute(x, "forward")
  print(y)
  local z = x:permute(x, "reverse")
  print(z)
  local chk_y = "[8, 2, 3, 9, 7, 1, 4, 10, 6, 0, 5]"
  local str_y = lRBC.pr(y)
  assert(str_y == chk_y)
  print("Test t2 completed successfully")
end
tests.t1()
-- return tests
