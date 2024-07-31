local lRBC = require 'lRBC'

local tests = {}

-- WARNING! Make sure that infile and outfile are sorted in same way
-- on week_start_date, co_loc_i
--
tests.prefix_sums = function(infile, outfile)
  local status
  local in_col_names =  { "x", }
  local in_qtypes = { "I1",  }
  local optargs = { is_hdr = true }
  local indf = assert(lRBC.read_csv(in_col_names, in_qtypes, infile, optargs))
  assert(type(indf) == "lRBC")
  local x = indf:get("x")
  local y = x:prefix_sums()
  local z = x:prefix_sums("forward")
  local w = x:prefix_sums("backward")
  local sx = tostring(x)
  local sy = tostring(y)
  local sz = tostring(z)
  local sw = tostring(w)
  assert(sx == "[1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1]")
  assert(sy == "[1, 0, 1, 2, 3, 4, 0, 0, 0, 0, 1, 0, 1, 2, 0, 0, 1]")
  assert(sz == "[1, 0, 1, 2, 3, 4, 0, 0, 0, 0, 1, 0, 1, 2, 0, 0, 1]")
  assert(sw == "[1, 0, 4, 3, 2, 1, 0, 0, 0, 0, 1, 0, 2, 1, 0, 0, 1]")

  print("Test prefix_sums completed successfully")
end
local infile = "data_prefix_sums.csv"
tests.prefix_sums(infile)
-- return tests
