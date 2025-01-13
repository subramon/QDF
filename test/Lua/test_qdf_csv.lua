local lQDF = require 'lQDF'
local tests = {}

tests.read_csv_1 = function ()
  local ncols = 40
  local M =  {}
  for i = 1, ncols do
    local m = {
    name = "col" .. tostring(i),
    qtype = "I4",
    is_load = true,
    has_nulls = false,
  }
    M[#M+1] = m
  end
  M[3].qtype = "F4"
  M[4].qtype = "F4"
  M[5].qtype = "F4"
  local col_names = {}
  for i = 1, #M do
    col_names[i] = M[i].name
  end
  local qtypes = {}
  for i = 1, #M do
    qtypes[i] = M[i].qtype
  end
  local infile = "in1.csv"
  local optargs = { is_hdr = true }
  local x = assert(lQDF.read_csv(M, infile, optargs))
  assert(type(x) == "lQDF")
  assert(x:jtype() == "j_object")
  local K = x:keys()
  -- for k, v in pairs(K) do print(k, v) end 
  for k1, v1 in pairs(K) do 
    local found = false
    for k2, v2 in pairs(col_names) do 
      if ( v2 == v1 ) then
        found = true; break
      end
    end
    assert(found)
  end
  for i, v in ipairs(col_names) do
    local y = x:get(v)
    assert(type(y) == "lQDF")
    assert(y:jtype() == "j_array")
    assert(y:qtype() == qtypes[i])
  end

  print("Test read_csv_1 completed successfully")
end
tests.read_csv_1()
-- return tests
