local lQDF = require 'lQDF'
local tests = {}

tests.add_col_to_df = function ()
  local infile = "../data/add_col_to_df1.csv"
  local optargs = { is_hdr = true }

  local M = {}
  M[#M+1] = { name = "f1", qtype = "F4", is_load = true, has_nulls = false,}
  M[#M+1] = { name = "f2", qtype = "F8", is_load = true, has_nulls = false,}
  M[#M+1] = { name = "sc", qtype = "SC:6", is_load = true, has_nulls = false,}
  M[#M+1] = { name = "w", qtype = "I1", is_load = true, has_nulls = false,}

  local T = assert(lQDF.read_csv(M, infile, optargs))
  local f1 = T:get("f1")
  assert(type(f1) == "lQDF")
  assert(f1:jtype() == "j_array")
  assert(f1:qtype() == "F4")
  local n = f1:num_elements()

  -- create a QDF that we want to add to T 
  local T2 = T:add_col_to_df("new_f1", f1)
  local K2 = T2:keys()
  local found = false
  for k, v in ipairs(K2) do 
    if ( v == "new_f1" ) then found = true; break end 
  end
  assert(found)
  assert(T2:check())

  local newcol = lQDF.const("ABC", "SC", n, n)
  local T3 = T2:add_col_to_df("str_col", f1)
  local K3 = T3:keys()
  local found = false
  for k, v in ipairs(K3) do 
    if ( v == "str_col" ) then found = true; break end 
  end
  assert(found)
  assert(T3:check())

  print("Test add_col_to_df completed successfully")
end
tests.add_col_to_df()
-- return tests
