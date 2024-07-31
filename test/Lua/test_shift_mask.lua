G = {} -- to suprpess strict
G.debug= true
require 'strict'
local lRBC = require 'lRBC'

local tests = {}
tests.t1  = function()
  local n = 10
  local start = 1
  local incr  = 1
  local qtypes = { "I4", "I8", }
  for _, qtype in ipairs(qtypes) do
    local x = lRBC.seq(start, incr, qtype, n)
    local str_x = lRBC.pr(x)
    assert(str_x == "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]")

    local w1 = x:shift_left(16)
    local w2 = w1:bitwise_and(1048576-1)
    local str_w2 = lRBC.pr(w2)
    assert(str_w2 == "[65536, 131072, 196608, 262144, 327680, 393216, 458752, 524288, 589824, 655360]")

    local y = x:shift_left(16):bitwise_and(1048576-1)
    local w1 = y:shift_right(16)
    local str_w1 = lRBC.pr(w1)
    assert(str_w1 == "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]")

    local w = x:shift_right(0)
    local str_w = lRBC.pr(w)
    assert(str_w == "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]")

    local w = x:shift_right(1)
    local str_w = lRBC.pr(w)
    assert(str_w == "[0, 1, 1, 2, 2, 3, 3, 4, 4, 5]")
  end
  print("Completed test t1 successfully")
end
tests.t1()
-- return tests
