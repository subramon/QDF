require 'strict'
local ffi = require 'ffi'
local lRBC = require 'lRBC'
local qtypes = require 'qtypes'
local tests = {}

tests.binld = function ()
  local x = lRBC.binld("0.rbc")
  assert(type(x) == "lRBC")
  local b_is_df, n, sz = x:is_data_frame()
  assert(b_is_df == true)
  assert(n > 0)
  assert(n <= sz)
  local y = x:get("week_start_date"):mktime()
  x:pr_df_as_csv({ "sls_unit_q", "co_loc_i", "week_start_date"}, "_x.csv")
  assert(type(y) == "lRBC")
  assert(y:jtype() == "j_array")
  local z = y:vs_op("/", 24 * 60 * 60, "I4")
  print("Test binld completed successfully")
end
tests.binld()
-- return tests
