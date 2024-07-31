G = {} -- to circumvent strict 
G.debug= true
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

tests.ifxthenyelsez = function ()
  local col_names =  { "y", "z", "x", }
  local qtypes = { "F4", "F4", "I1", }
  local infile = "data_ifxthenyelsez.csv"
  local optargs = { is_hdr = true }
  local df = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
  assert(type(df) == "lRBC")
  local y = df:get("y")
  local z = df:get("z")
  local x = df:get("x")
  -- print(x)
  -- print(y)
  -- print(z)
  local w = lRBC.ifxthenyelsez(x, y, z)
  local str_w = tostring(w)
  -- print(w)
  assert(str_w == "[-10, 20, -30, 40, -50, 60, -70, 80, -90]")

  print("Test ifxthenyelsez completed successfully")
end
tests.ifxthenyelsez()
-- return tests
