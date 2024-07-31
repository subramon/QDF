G = {} --- to suppress strict
G.debug= true
require 'strict'
require 'strict'
local lRBC = require 'lRBC'
local chk1 = require 'mgcv/chk1'
local tests = {}

tests.test1 = function ()
  local col_names =  { "junk", "sls_unit_q", }
  local qtypes = { "I1", "F4", }
  local infile = "data_chk1.csv"
  local optargs = { is_hdr = true, }
  local x = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
  local status = chk1(x, nil, "sls_unit_q", 10, 0.95, 2)

  print("Test test1 completed successfully")
end
tests.test1()
-- return tests
