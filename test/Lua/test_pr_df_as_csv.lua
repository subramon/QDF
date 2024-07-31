local lRBC = require 'lRBC'
local tests = {}

tests.test1 = function ()
  local col_names =  { "date","val", "co_loc_i", 
    "l1", "l2", "l3", "l4", "l5", 
    "nn_l1", "nn_l2", "nn_l3", "nn_l4", "nn_l5" }
  local qtypes = { "TM:%Y-%m-%d", "F4", "I2", 
    "F4", "F4", "F4", "F4", "F4", 
    "I1", "I1", "I1", "I1", "I1", }
  local ncols  = #col_names
  assert(#qtypes == #col_names)
  local infile = "../../data/lags1.csv"
  local optargs = { is_hdr = true }
  local x = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
  assert(type(x) == "lRBC")
  x:pr_df_as_csv({"l1", "nn_l1", "l2", "nn_l2", }, "/tmp/foo.csv")
  -- TODO Should test output 
  print("Test test1 completed successfully")
end
tests.test1()
-- return tests
