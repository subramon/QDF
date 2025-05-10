local lQDF = require 'lQDF'

local tests = {}

-- WARNING! Make sure that infile and outfile are sorted in same way
-- on week_start_date, co_loc_i
--
tests.smear = function(infile, outfile)
  local status
  local in_col_names =  { "x1", "off1", }
  local in_qtypes = { "I4", "I4",  }
  local optargs = { is_hdr = true }
  local M = {}
  for k, v in ipairs(in_col_names) do
    M[k] = { name = in_col_names[k], qtype = in_qtypes[k], }
  end
  local indf = assert(lQDF.read_csv(M, infile, optargs))
  assert(type(indf) == "lQDF")
  local x = indf:get("x1")
  local o = indf:get("off1")
  local y = x:smear(o)
  local z = x:smear(o, "forward")
  local w = x:smear(o, "backward")
  local sx = tostring(x)
  local so = tostring(o)
  local sy = tostring(y)
  local sz = tostring(z)
  local sw = tostring(w)
  assert(sw == "[1, 1, 3, 3, 3, 3, 7, 7, 7, 10, 10, 12, 12, 14]")
  assert(sy == "[1, 3, 3, 5, 7, 9, 7, 9, 11, 10, 12, 12, 14, 14]")
  assert(sy == sz)

  print("Test smear completed successfully")
end
local infile = "data_smear.csv"
tests.smear(infile)
-- return tests
