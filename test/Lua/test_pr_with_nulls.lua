local lQDF = require 'lQDF'

local tests = {}
tests.t1 = function()
  local x = lQDF.binld("hist.qdf")
  assert(x:check())
  assert(x:is_data_frame())
  local K = x:keys()
  assert(type(K) == "table")
  -- for k, v in pairs(K) do print(k, v) end 
  x:pr_df_as_csv(nil, "_x.csv")

  print("Test t1 succeeded ")
end
tests.t1()
