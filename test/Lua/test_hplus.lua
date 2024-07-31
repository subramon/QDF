require 'strict'
local lRBC = require 'lRBC'
local hplus = require 'hplus'
-- here we fake out the real configs 
local  lC = { hplus = { 
  cutoff_date = "2017-02-26",
  time_fld = "week_start_date",
  format = "%Y-%m-%d", 
  }
}
--===================
local cols = { "co_loc_i", "sls_unit_q", "week_start_date"}
local qtypes = { "I2",  "F4", "TM:%Y-%m-%d"}
local optargs = { is_hdr = true }

local cutoff_sec = 1488096000 -- created as date %s <C.hplus.cutoff_date>
local num_orig = 1675 -- created as: wc data_hplus_orig_1.csv
 -- wc C.hplus.cutoff_date orig_file 
if ( optargs.is_hdr ) then 
  num_orig = num_orig - 1 
end
local num_to_lose = 8 
  -- created as: grep "2017-02-26" data_hplus_orig_1.csv | wc

local tests = {}
tests.t1= function()
  local orig_file = "data_hplus_orig_1.csv"
  local orig_df = lRBC.read_csv(cols, qtypes, orig_file, optargs)
  orig_df:bindmp("_x.rbc")
  --===================
  local incr_file = "data_hplus_incr_1.csv"
  local incr_df = lRBC.read_csv(cols, qtypes, incr_file, optargs)
  --===================
  local D = hplus(lC, "_x.rbc", incr_df)
  assert(type(D) == "table")
  -- for k, v in pairs(D) do print(k, v) end 
  assert(D.tsec        == cutoff_sec)
  local num_to_gain = 10 -- created as: wc data_hplus_incr_1.csv
  -- wc incr_file 
  if ( optargs.is_hdr ) then 
    num_to_gain = num_to_gain - 1 
  end
  assert(D.num_to_expand == (num_to_gain - num_to_lose))
  assert(D.num_to_keep == num_orig - num_to_lose)
  assert(D.sz_old_df   == num_orig)
  assert(D.n_old_df    == num_orig)
  
  -- print(D.sz_old_df, D.post_sz_old_df)
  assert(D.sz_old_df   == D.post_sz_old_df)
  assert(D.new_n    == num_orig - num_to_lose + num_to_gain)
  -- test case for return of 1 
  local err_code = hplus(lC, "_x.rbc", incr_df)
  assert(type(err_code) == "number")
  assert(err_code == 1)
  print("test hplus 1 executed successfully")
end
tests.t2 = function()
  -- In this test, we try and add a dataframe to itself
  -- It should fail because it violates the constraint that the
  -- time periods of the two dataframes (original and incremental) cannot
  -- overlap 
  local orig_file = "data_hplus_orig_1.csv"
  local orig_df = lRBC.read_csv(cols, qtypes, orig_file, optargs)
  orig_df:bindmp("_x.rbc")
  orig_df:bindmp("_y.rbc")
  --===================
  local incr_df = lRBC.binld("_y.rbc", false)
  local err_code = hplus(lC, "_x.rbc", incr_df)
  assert(err_code == 1)
  print("test hplus 2 executed successfully")
end


tests.t1()
tests.t2()
