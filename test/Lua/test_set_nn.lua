G = {} -- to circumvent strict 
G.debug= true
require 'strict'
local lQDF = require 'lQDF'
local tests = {}

tests.set_nn = function ()
  local col_names =  { "f1", "nn_f1", }
  local qtypes = { "F4", "I1", }
  local infile = "data_set_nn.csv"
  local optargs = { is_hdr = true }
  local x = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))
  assert(type(x) == "lQDF")
  local f1 = x:get("f1")
  local nn_f1 = x:get("nn_f1")
  f1:set_nn(nn_f1)
  assert(f1:has_nn())
  assert(f1:check())

  print("Test set_nn completed successfully")
end
tests.set_nn()
-- return tests
