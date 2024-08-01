G = {} -- to circumvent strict 
G.debug= true
require 'strict'
require 'strict'
local lQDF = require 'lQDF'
local tests = {}

tests.test1 = function ()
  local col_names =  { "junk", "sls_unit_q", }
  local qtypes = { "I1", "F4", }
  local infile = "data_chk1.csv"
  local optargs = { is_hdr = true, }
  local x = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))

  local y = x:clone()
  assert(type(y) == "lQDF")
  assert(y:check())

  local z = y:get("sls_unit_q")
  local w = z:clone()
  assert(type(w) == "lQDF")
  assert(w:check())

  print("Test test1 completed successfully")
end
tests.test1()
-- return tests
