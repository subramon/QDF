local lQDF = require 'lQDF'
local tests = {}

tests.where = function ()
  local infile = "../data/where1.csv"
  local optargs = { is_hdr = true }

  local M = {}
  M[#M+1] = { name = "f1", qtype = "F4", is_load = true, has_nulls = false,}
  M[#M+1] = { name = "f2", qtype = "F8", is_load = true, has_nulls = false,}
  M[#M+1] = { name = "sc", qtype = "SC:6", is_load = true, has_nulls = false,}
  M[#M+1] = { name = "w", qtype = "I1", is_load = true, has_nulls = false,}

  local T = assert(lQDF.read_csv(M, infile, optargs))
  assert(type(T) == "lQDF")
  assert(T:check())
  assert(T:jtype() == "j_object")
  local f1 = T:get("f1")
  assert(type(f1) == "lQDF")
  assert(f1:jtype() == "j_array")
  assert(f1:qtype() == "F4")
  assert(f1:num_elements() == 4)

  local w = T:get("w")
  local wprime = w:clone()
  local Tout, n = T:where(wprime)
  assert(type(Tout) == "lQDF")
  assert(Tout:check())
  assert(type(n) == "number")
  assert(n == 2)

  local f2 = Tout:get("f2")
  assert(type(f2) == "lQDF")
  assert(f2:check())
  assert(f2:jtype() == "j_array")
  assert(f2:qtype() == "F8")
  f2:pr_csv()
  assert(f2:arr_len() == 2) -- note change
  local s = lQDF.pr(f2)
  print(s)
  assert(s == "[10, 30]")
  local nuq = f2:num_unique()
  assert(nuq == 2)

  print("Test where completed successfully")
end
tests.where()
-- return tests
