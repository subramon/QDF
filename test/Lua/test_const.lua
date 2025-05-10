G = {}  -- to suppress strict 
G.debug= true
require 'strict'
require 'strict'
local lQDF = require 'lQDF'

local tests = {}
tests.t1  = function()
  local n = 10
  local val = 1
  local x = lQDF.const(val, "I8", n)
  local y = lQDF.pr(x)
  assert(x:qtype() == "I8")
  assert(x:jtype() == "j_array")
  assert(y == "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1]")
  print("Completed test t1 successfully")
end
tests.t2  = function()
  local n = 10
  local start = 1
  local incr  = 1
  local qtypes = {"I2", "I4" }
  for _, qtype in ipairs(qtypes) do
    local x = lQDF.seq(start, incr, qtype, n)
    local y = lQDF.pr(x)
    assert(x:qtype() == qtype)
    assert(x:jtype() == "j_array")
    assert(y == "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]")
  end
  print("Completed test t2 successfully")
end
tests.t3  = function()
  local n = 10

  local y = lQDF.const(1, "I2", n)
  local z = y:shift_left(2)
  local sz = lQDF.pr(z)
  assert(sz == "[4, 4, 4, 4, 4, 4, 4, 4, 4, 4]")

  local y = lQDF.const(1, "I4", n)
  local z = y:shift_left(1)
  local sz = lQDF.pr(z)
  assert(sz == "[2, 2, 2, 2, 2, 2, 2, 2, 2, 2]")

  local y = lQDF.const(1, "I8", n)
  local z = y:shift_left(0)
  local sz = lQDF.pr(z)
  assert(sz == "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1]")

  print("Completed test t3 successfully")
end
tests.const_str  = function()
  local n = 10

  local y = lQDF.const("abc", "SC", 4, 8)
  -- y:pr_csv()
  assert(y:check())
  print("Completed test const_str successfully")
end
tests.t1()
tests.t2()
tests.t3()
tests.const_str()
-- return tests
