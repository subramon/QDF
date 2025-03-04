local lQDF = require 'lQDF'

local tests = {}
tests.t1 = function()
  local x = lQDF.binld("x.qdf", true)
  assert(x:check())
  assert(x:is_data_frame())
  local K = x:keys()
  for k, v in pairs(K) do print(k, v) end 
  local s = x:get("regular_price")
  assert(s:jtype() == "j_array")
  assert(s:qtype() == "F4")
  print(s:num_elements())
  assert(s:check())
  s:pr_csv("_s1.csv")
  print("=====================")
  local s2 = lQDF.binary_op(s, 1, "sub")
  assert(s:num_elements() == s2:num_elements())
  assert(s:qtype() == s2:qtype())
  assert(s:jtype() == s2:jtype())
  assert(s2:check())
  s2:pr_csv("_s2.csv")
  print("=====================")
  print("TRYING IN SITU");
  local rslt = lQDF.binary_op(s, 2, "sub", { in_situ = true })
  s:pr_csv("_s3.csv")

  local rslt = lQDF.binary_op(s, 2, "add", { in_situ = true })
  s:pr_csv("_s4.csv")
  local old_price = s:clone()
  --====================================================
  x:pr_df_as_csv(nil, "_df1.csv")
  --====================================================
  -- load a vector from CSV file 
  local M = {}
  M[#M+1] = { name =  "regular_price", qtype = "F4", 
    is_load = true, has_nulls = false, }
  local infile    = "../data/new_regular_price.csv"
  local optargs   = { is_hdr = true }

  local tmp = assert(lQDF.read_csv(M, infile, optargs))
  assert(type(tmp) == "lQDF")
  assert(tmp:jtype() == "j_object")
  local new_price = tmp:get("regular_price")
  assert(new_price:jtype() == "j_array")
  assert(new_price:qtype() == "F4")
  assert(new_price:num_elements() == s:num_elements())
  -- print(x)
  --====================================================
  -- test place in data frame 
  x:place_in_data_frame("regular_price", new_price)
  x:pr_df_as_csv(nil, "_df2.csv")
  x:place_in_data_frame("regular_price", old_price)
  x:pr_df_as_csv(nil, "_df3.csv")
  print("Test t1 succeeded ")
end
tests.t1()
