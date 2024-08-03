G = {} -- to circumvent strict 
G.debug= true
require 'strict'
local lQDF = require 'lQDF'
local tests = {}
tests.pr_csv = function()
  local T = {}
  local n = 10
  for i = 1, n do 
    T[#T+1] = i
  end
  local x = lQDF(T)
  assert(type(x) == "lQDF")
  print(x)
  x:pr_csv("/tmp/_x")
  print("Test pr_csv completed ")
end
tests.pr_csv()
