G = {} -- to circumvent strict 
G.debug= true
require 'strict'
local lQDF = require 'lQDF'
local tests = {}

tests.ifxthenyelsez = function ()
  local col_names =  { "y", "z", "x", }
  local qtypes = { "F4", "F4", "I1", }
  local infile = "data_ifxthenyelsez.csv"
  local optargs = { is_hdr = true }
  local M = {}
  for k, v in ipairs(col_names) do
    M[k] = { name = col_names[k], qtype = qtypes[k], }
  end
  local df = assert(lQDF.read_csv(M, infile, optargs))
  assert(type(df) == "lQDF")
  local y = df:get("y")
  local z = df:get("z")
  local x = df:get("x")
  -- print(x)
  -- print(y)
  -- print(z)
  local w = lQDF.ifxthenyelsez(x, y, z)
  local str_w = tostring(w)
  -- print(w)
  assert(str_w == "[-10, 20, -30, 40, -50, 60, -70, 80, -90]")

  print("Test ifxthenyelsez completed successfully")
end
tests.ifxthenyelsez()
-- return tests
