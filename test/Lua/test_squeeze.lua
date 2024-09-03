local lQDF = require 'lQDF'
local tests = {}

tests.squeeze = function ()
  local col_names =  { "f1","f2", "w", }
  local qtypes = { "I4", "F8", "I1", }
  local ncols  = #col_names
  assert(#qtypes == #col_names)
  local infile = "../data/squeeze1.csv"
  local optargs = { is_hdr = true }
  local x = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))
  assert(type(x) == "lQDF")
  assert(x:jtype() == "j_object")
  local y = x:get("f1")
  assert(type(y) == "lQDF")
  assert(y:jtype() == "j_array")
  assert(y:qtype() == "I4")
  assert(y:num_elements() == 4)
  -- print(x)

  local w = x:get("w")
  local n = x:squeeze_where(w)
  assert(n == 2)
  local y = x:get("f2")
  assert(type(y) == "lQDF")
  assert(y:check())
  assert(y:jtype() == "j_array")
  assert(y:qtype() == "F8")
  assert(y:arr_size() == 4) -- note no change
  assert(y:arr_len() == 2) -- note change
  local s = lQDF.pr(y)
  print(s)
  assert(s == "[10, 30]")
  local nuq = y:num_unique()
  assert(nuq == 2)

  print("Test squeeze completed successfully")
end
tests.squeeze()
-- return tests
