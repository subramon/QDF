G = {} -- to circumvent strict 
G.debug= true
require 'strict'
require 'strict'
local lQDF = require 'lQDF'
local tests = {}

tests.coalesce = function ()
  local col_names =  { "f1", "nn_f1", "f2", "nn_f2", "f3", "nn_f3", }
  local qtypes = { "F4", "I1", "F4", "I1", "F4", "I1", }
  local ncols  = #col_names
  assert(#qtypes == #col_names)
  local infile = "data_coalesce.csv"
  local optargs = { is_hdr = true }
  local x = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))
  assert(type(x) == "lQDF")
  
  -- print(x)
  local f1 = x:get("f1"); f1:set_nn(x:get("nn_f1"))
  local f2 = x:get("f2"); f2:set_nn(x:get("nn_f2"))

  local f3, num_nulls = lQDF.coalesce(f1, f2)
  if ( num_nulls > 0 ) then
    assert(f3:has_nn())
  else
    assert(not f3:has_nn())
  end

  local chk_f3 = x:get("f3")
  -- Compare f3 and chk_f3
  local s1 = tostring(x:get("f3"))
  local s2 = tostring(chk_f3)
  assert(s1 == s2)
  assert(num_nulls == 1 ) -- for this specific example
  -- Compare nn_f3 and chk_nn_f3
  if ( num_nulls > 0 ) then 
    local chk_nn_f3 = x:get("nn_f3")
    local nn_f3 = f3:get_nn()
    local s1 = tostring(nn_f3)
    local s2 = tostring(chk_nn_f3)
    assert(s1 == s2)
    -- print(s1)
    -- print(s2)

  end

  print("Test coalesce completed successfully")
end
tests.coalesce()
-- return tests
