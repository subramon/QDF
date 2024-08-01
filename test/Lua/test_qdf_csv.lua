local lQDF = require 'lQDF'
local tests = {}

tests.read_csv_1 = function ()
  local ncols = 40
  local col_names =  {}
  for i = 1, ncols do
    col_names[i] = "col" .. tostring(i)
  end
local qtypes = { "I4", "I2", "F4", "F4", "F4", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I4", "I2", "I1", }
  assert(#qtypes == #col_names)
  local infile = "in1.csv"
  local optargs = { is_hdr = true }
  local x = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))
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
