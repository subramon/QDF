G = {} -- to suppress strict 
require 'strict'
local lQDF = require 'lQDF'

local tests = {}
tests.t1  = function()
  local n = 10
  local start = 1
  local incr  = 1
  local qtypes = {"I8", }
  for _, qtype in ipairs(qtypes) do
    local x = lQDF.seq(start, incr, qtype, n)
    local y = lQDF.pr(x)
    assert(y == "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]")

    x:sort("ascending")
    local y = lQDF.pr(x)
    assert(y == "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]")

    x:sort("descending")
    local y = lQDF.pr(x)
    assert(y == "[10, 9, 8, 7, 6, 5, 4, 3, 2, 1]")

  end
  print("Completed test t1 successfully")
end
tests.t1()
-- return tests
