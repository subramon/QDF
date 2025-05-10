G = {} -- to avoid strict complaints
G.debug= true
require 'strict'
local lQDF = require 'lQDF'
local tests = {}

tests.test_bitwise_not = function()
  --  testing bitwise not 
  local n = 10
  for _q, qtype in ipairs({"I1", "I2", "I4", "I8", }) do 
    local y = lQDF.seq(0, 1, qtype, n)
    local noty = y:bitwise_not()
    assert(noty:jtype() == "j_array")
    assert(noty:qtype() == qtype)
    local good_noty_str = "[-1, -2, -3, -4, -5, -6, -7, -8, -9, -10]"
    local noty_str = lQDF.pr(noty)
    assert(good_noty_str == noty_str)
  end
  print("Test bitwise_not completed successfully")
end

tests.test_logical_not = function()
  --  testing logical not 
  local n = 10
  for _q, qtype in ipairs({"I1", "I2", "I4", "I8", }) do 
    local y = lQDF.const(1, qtype, n)
    local noty = y:logical_not()
    assert(noty:qtype() == "I1")
    assert(noty:jtype() == "j_array")
    assert(noty:max() <= 1)
    assert(noty:min() >= 0)

    local z = lQDF.const(0, qtype, n)
    local notz = z:logical_not()
    assert(notz:qtype() == "I1")
    assert(notz:jtype() == "j_array")
    assert(notz:max() <= 1)
    assert(notz:min() >= 0)
    -- noty == z and notz == y
    if ( qtype == "I1" ) then 
      local m = y:eq(notz):sum(); assert(n == m)
      local m = z:eq(noty):sum(); assert(n == m)
    end
    -- noty and notyy should be same 
    local yy = lQDF.const(123, qtype, n)
    local notyy = yy:logical_not()
    assert(notyy:qtype() == "I1")
    assert(notyy:jtype() == "j_array")
    assert(notyy:max() <= 1)
    assert(notyy:min() >= 0)
    local m = noty:eq(notyy):sum(); assert(n == m)
  end
  print("Test logical_not completed successfully")
end

tests.test_bitwise_op = function ()
  local col_names =  { "f1","f2", }
  local qtypes = { "I1", "I1", }
  local ncols  = #col_names
  assert(#qtypes == #col_names)
  local infile = "../data/logical_op1.csv"
  local optargs = { is_hdr = true }
  local M = {}
  for k, v in ipairs(col_names) do
    M[k] = { name = col_names[k], qtype = qtypes[k], }
  end
  local x = assert(lQDF.read_csv(M, infile, optargs))
  assert(type(x) == "lQDF")
  -- print(x)
  assert(x:jtype() == "j_object")
  local y = x:get("f1")
  assert(type(y) == "lQDF")
  assert(y:jtype() == "j_array")
  assert(y:qtype() == "I1")
  assert(y:num_elements() == 4)
  local z = x:get("f2")


  -- print("y = "); print(y)
  -- print("z = "); print(z)
  local band = y:bitwise_and(z)
  local s1 = lQDF.pr(band)
  -- print("y and z = ")
  -- print(s1)
  assert(s1 == "[0, 0, 0, 1]")

  local bor = y:bitwise_or(z)
  local s2 = lQDF.pr(bor)
  -- print("y or z = ")
  -- print(s2)
  -- assert(s2 == "[0, 1, 1, 1]")

  local bxor = y:bitwise_xor(z)
  local s3 = lQDF.pr(bxor)
  -- print("y xor z = ")
  -- print(s3)
  -- assert(s3 == "[0, 1, 1, 0]")

  print("Test bitwise_op completed successfully")
end
tests.test_bitwise_op()
tests.test_logical_not()
tests.test_bitwise_not()
-- return tests
