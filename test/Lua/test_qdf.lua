G = {}
G.debug= true
require 'strict'
local simdjson = require 'simdjson'
local ffi = require 'ffi'
local lQDF = require 'lQDF'
local tests = {}

local function trim1(s)
   return (s:gsub("^%s*(.-)%s*$", "%1"))
end

local function deep_copy(x)
  assert(type(x) == "table")
  local y = {}
  for k, v in pairs(x) do 
    assert(type(k) == "string")
    if ( ( type(v) == "number" ) or 
         ( type(v) == "string" ) or 
         ( type(v) == "boolean" ) ) then
         y[k] = v
     elseif ( type(v) == "table") then
       y[k] = deep_copy(v)
     else
         error("XX")
     end
   end
   return y
 end



local function plp(x, val_to_compare)

  assert(type(x) == "lQDF")
  x:check()

  local y = x:get()
  assert(y == val_to_compare)

  local z = x:get(nil, "raw")
  z:check()
  assert(type(z) == "lQDF")
  assert(z:get() == x:get())

  local w = assert(x:clone())
  assert(type(w) == "lQDF")
  w:check()
  x = nil; collectgarbage()
  z:check()
  w:check()
  local zval = z:get()
  local wval = w:get()
  assert(z:get() == w:get())

end

tests.primitive = function()
  local x, y, z

  local json_str = "null"
  local json = assert(simdjson.parse(json_str))
  -- print(type(json))
  x = lQDF(json)
  plp(x, nil)

  --==================================
  local json_str = "true"
  local json = assert(simdjson.parse(json_str))
  assert(type(json) == "boolean")
  x = lQDF(json)
  assert(x:jtype() == "j_bool")
  plp(x, true)
  --==================================
  local json_str = "false"
  local json = simdjson.parse(json_str)
  assert(type(json) == "boolean")
  x = lQDF(json)
  assert(x:jtype() == "j_bool")
  assert(x:bool_val() == false)
  plp(x, false)
 
  --==================================
  local json_str = "12345"
  local json = assert(simdjson.parse(json_str))
  assert(type(json) == "number")
  x = lQDF(json)
  assert(x:jtype() == "j_number")
  assert(x:num_val() == 12345)
  plp(x, 12345)
  --==================================
  local json_str = '"some_string"'
  local json = assert(simdjson.parse(json_str))
  assert(type(json) == "string")
  x = lQDF(json)
  assert(x:jtype() == "j_string")
  plp(x, "some_string")
  --==================================
  print("Test primitive successfully completed")
end

tests.str_array = function()
  local json_str = '[ "a", "bc", "def", "ghij", "klmno"]'
  local json = simdjson.parse(json_str)
  assert(type(json) == "table")
  local x = lQDF(json)
  -- print(type(x))
  assert(type(x) == "lQDF")
  assert(x:check())
  local str = lQDF.pr(x)
  assert(str == '["a", "bc", "def", "ghij", "klmno"]')
  local y = x:get(2)
  -- print("second item is ", y)
  assert(y == "def")
  -- get raw
  local yprime = x:get(2, "raw")
  assert(type(yprime) == "lQDF")
  assert(yprime:jtype() == "j_string")
  --
  -- TODO P3 check on yprime 
  -----
  local z = x:clone()
  assert(z:check())
  z:set_name("z is clone of x")
  -- print("ZZZ", z:name() , "z is clone of x")
  assert(z:name() == "z is clone of x")
  assert(type(z) == "lQDF")
  assert(z:check())
  local v = z:clone()
  v:check()
  assert(type(v) == "lQDF")

  local w = x:get()
  x = nil
  collectgarbage()
  w:check()
  assert(type(w) == "lQDF")
  -- print("wget", w:get(2))
  -- print("zget", z:get(2))
  assert(w:get(2) == z:get(2))

  print("Test str_array completed successfully")
end
tests.bool_array = function()
  local x, y, z, w
  local str_json = '[ true, false, true, false, true]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  x = lQDF(json)
  assert(type(x) == "lQDF")
  print("x qtype = ", x:qtype())
  assert(x:qtype() == "BL")
  print(x)
  local str = lQDF.pr(x)
  assert(str == "[true, false, true, false, true]")

  y = x:get(2)
  assert(x:qtype() == "BL")
  assert(y == true)

  y = x:get(1)
  assert(y == false)

  print("Test bool_array completed successfully")
end
tests.num_array = function()
  local x, y, z, w
  local str_json = '[ 10, 20, 30, 40, 50, 60]'
  local json = simdjson.parse(str_json)
  assert(type(json) == "table")
  for k, v in pairs(json) do print(k, v) end 
  x = lQDF(json)
  assert(type(x) == "lQDF")
  assert(x:qtype() == "F8")
  print("YYYYYY", x)
  local str = lQDF.pr(x)
  print("XXXXX", str)
  assert(str == "[10, 20, 30, 40, 50, 60]")
  -- make y as a copy of x 

  y = x:get()
  assert(type(y) == "lQDF")
  assert(y:qtype() == "F8")
  -- delete y and make sure x has not changed 
  y = nil
  collectgarbage()
  local str = lQDF.pr(x)
  assert(str == "[10, 20, 30, 40, 50, 60]")
  
  y = x:get(2)
  assert(x:qtype() == "F8")
  assert(y == 30)
  
  -- get raw
  local yprime = x:get(2, "raw")
  assert(type(yprime) == "lQDF")
  assert(yprime:jtype() == "j_number")
  -----
  local z = x:clone()
  z:check()
  local w = x:get()
  x = nil
  collectgarbage()
  w:check()
  assert(type(w) == "lQDF")
  assert(w:get(2) == z:get(2))

  print("Test num_array completed successfully")
end

tests.array = function()
  local json_str = '[ 10, true, "some_string" ]'
  local json = simdjson.parse(json_str)
  assert(type(json) == "table")
  local x = lQDF(json)
  assert(type(x) == "lQDF")
  print(x) 
  print("Test array completed successfully")
end

tests.t2 = function()
  local json_str = '12345'
  local json = simdjson.parse(json_str)
  assert(type(json) == "number") 
  local x = lQDF(json)
  assert(type(x) == "lQDF")
  local y = x:get(nil, "raw")
  assert(type(y) == "lQDF")
  local z = y:set(54321)
  assert(type(z) == "lQDF")
  z:check()
  local valx =  lQDF.pr(x)
  local valy =  lQDF.pr(y)
  local valz =  lQDF.pr(z)
  assert(valx == valy)
  assert(valx == "12345")
  assert(valz == "54321")
  print("Test t2 succeeded")
end

tests.t3 = function()
  -- checking how gc works 
  local ffi = require 'ffi'
  local sz = 1048576
  local n = 1024
  local x  = {}
  local y  = {}
  -- IMPORTANT: set expt = 1 => seg fault; 2=> okay
  local expt = 2
  for i = 1, n do
    local cdata = ffi.gc(ffi.C.malloc(sz), ffi.C.free)
    -- x[i] = { data = cdata } 
    x[i] = cdata
    if ( expt == 1 ) then 
      local tmp = ffi.cast("char *", x[i])
      y[i] = tmp
    elseif ( expt == 2 ) then 
      y[i] = x[i]
    else
      error("")
    end
  end
  x = nil; collectgarbage()
  for i = 1, n do
    -- local tmp = ffi.cast("char *", y[i].data)
    local tmp = ffi.cast("char *", y[i])
    for j = 1, sz do 
      local z = tmp[j]
    end
  end
  print("Test t3 succeeded")
end
tests.t4 = function()
  -- checking how gc works 
  local ffi = require 'ffi'
  local sz = 1048576
  local n = 1024
  local x  = {}
  local y  = {}
  -- IMPORTANT: set expt = 1 => seg fault; 2=> okay
  local expt = 2
  for i = 1, n do
    local cdata = ffi.gc(ffi.C.malloc(sz), ffi.C.free)
    x[i] = { data = cdata } 
    y[i] = { parent = x[i] }
  end
  for i = 1, n do
    x[i] = nil
  end
  x = nil; collectgarbage()
  bigsum = 0
  for i = 1, n do
    local tmp = ffi.cast("char *", y[i].parent.data)
    local sum = 0
    for j = 1, sz do 
      local z = tmp[j]
      sum = sum + z 
    end
    bigsum = bigsum + sum
  end
  print("Test t4 succeeded")
end

tests.nested_array = function() -- testing  arrays within arrays
  local json_str = '[ 1, true, "str1", [ 2, false, "str2"] ]'
  local json = simdjson.parse(json_str)
  assert(type(json) == "table")
  local x = lQDF(json)
  assert(type(x) == "lQDF")
  print(x) 
  print("Test nested_array succeeded")
end

tests.doubly_nested_array = function() -- testing  arrays within arrays
  local json_str = '[ 1, true, "str1", [ 2, false, "str2", [ 3, "str3" ] ] ]'
  local json = simdjson.parse(json_str)
  assert(type(json) == "table")
  local x = lQDF(json)
  assert(type(x) == "lQDF")
  print("x = ", lQDF.pr(x))

  local y = x:get(3)
  assert(type(y) == "lQDF")
  print("y = ", lQDF.pr(y))

  local z = y:get(3)
  assert(type(z) == "lQDF")
  print("z = ", lQDF.pr(z))

  local w = z:get(1)
  print("w = ", w)
  print("type(w) = ", type(w))
  assert(type(w) == "string")
  assert(w == "str3")

  local w = y:get(0)
  assert(type(w) == "number")
  assert(w == 2)

  print("Test doubly_nested_array succeeded")
end

tests.json_as_table = function()
  local x = { 10, 20, 30, 40, 50 }
  local y = lQDF(x)
  assert(type(y) == "lQDF")
  print(y) 
  print("Test json_as_table succeeded")
end
tests.json_as_table_2 = function()
  local x = { 10, 20, 30, 40, 50, { 10, 20, 30, 40, 50, } }
  local y = lQDF(x)
  assert(type(y) == "lQDF")
  print(y) 
  print("Test json_as_table_2 succeeded")
end
tests.json_as_table_3 = function()
  local x = { 0, 10, 20, 30, 40, 50, { 100, 200, 300, 400, { 1000, 2000 } } }
  local y = lQDF(x)
  assert(type(y) == "lQDF")

  print(y) 
  assert(y:qtype() == "Q0")
  assert(y:num_elements() == 7)

  local z = y:get(5)
  assert(type(z) == "number")
  assert(z == 50)

  local z = y:get(6)
  assert(type(z) == "lQDF")
  local w = z:get(4)
  assert(type(w) == "lQDF")
  print(w)

  local v = w:get(0)
  assert(type(v) == "number")
  assert(v == 1000)

  print("Test json_as_table_3 succeeded")
end
tests.object_1 = function()
  local x = { } 
  x["k1"] = 123
  x["k2"] = 123.45
  x["k3"] = true
  x["k4"] = false
  x["k5"] = "123.45"
  local nx = 0
  for k, v in pairs(x) do 
    nx = nx + 1 
  end
  local y = lQDF(x)
  assert(type(y) == "lQDF")
  assert(y:num_keys() == nx)
  local K = y:keys()
  -- check number of keys
  assert(#K == nx)
  -- check keys are distinct
  for i = 1, #K do 
    for j = i+1, #K do 
      assert(K[i] ~= K[j])
    end
  end
  -- check keys are same as what went in
  for i = 1, #K do 
    local is_found = false
    local k_i = K[i]
    for k, v in pairs(x) do 
      if ( k_i == k ) then 
        is_found = true
        break
      end
    end
    assert(is_found)
  end
  for k, v in pairs(x) do 
    assert(y:is_key(k))
  end
  assert(not y:is_key("some_non_existent_key"))
  -- Check that values of keys are same as what went in 
  for k, v in pairs(x) do 
    local chk_v = y:get(k)
    assert(v == chk_v)
  end
  --================================
  print("Test object_1 succeeded")
end
tests.variable_length_strings_in_array = function()
  local x = { "a", "bc", "def", "ghij", "kmlno" }
  local y = lQDF(x)
  assert(type(y) == "lQDF")
  print(y) 
  for i = 1, #x do
    local z = y:get(i-1)
    assert(z == x[i])
  end
  print("Test variable_length_strings_in_array succeeded")
end
tests.sum_num_array = function()
  local x = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
  local y = assert(lQDF(x))
  local sum = y:sum()
  assert(type(sum) == "number")
  assert(sum == 45)
  -- check by pulling out each iteme and summing in Lua 
  sum = 0
  for i = 1, #x do
    sum = sum + y:get(i-1)
  end
  assert(sum == 45)
  --==== test min
  local min = y:min()
  assert(type(min) == "number")
  assert(min == 0)

  --==== test max
  local max = y:max()
  assert(type(max) == "number")
  assert(max == 9)


  print("Test sum_num_array succeeded")
end
tests.sum_mixed_array = function()
  local x = { 0, 1, 2, 3, 4, "5", 6, 7, 8, 9 }
  local y = assert(lQDF(x))
  print(">>> START Deliberate error")
  local status, sum = pcall(lQDF.f_to_s, y, "sum")
  assert(not status)
  print("<<< STOP  Deliberate error")
  print("Test sum_mixed_array succeeded")
end
tests.vals_num_array = function()
  local x = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
  local y = assert(lQDF(x))
  local n, dptr = y:get_arr_ptr()
  assert(n == #x)
  dptr = ffi.cast("double *", dptr)
  local sum = 0
  for  i = 1, n do
    sum = sum + dptr[i-1]
  end
  assert(sum == 45)
  print("Test vals_num_array succeeded")
end
tests.nested_object = function()
  local x = { } 
  x["k1"] = 123
  x["k2"] = 123.45
  x["k3"] = true
  x["k4"] = false
  x["k5"] = "123.45"
  x["k6"] = deep_copy(x)
  local nx = 0
  for k, v in pairs(x) do 
    nx = nx + 1 
  end
  local y = lQDF(x)
  local z = y:get("k6")
  assert(type(y) == "lQDF")
  local w = z:get("k5")
  assert(type(w) == "string")
  assert(w == "123.45")
  print("Test nested_object succeeded")
end

tests.primitive()
tests.str_array() 
tests.num_array()
--[[
tests.array()
tests.bool_array()
tests.json_as_table()
tests.json_as_table_2()
tests.json_as_table_3()
tests.nested_array()
tests.doubly_nested_array()
tests.sum_num_array()
tests.sum_mixed_array()
tests.vals_num_array()
tests.object_1()
tests.nested_object()
tests.variable_length_strings_in_array()  -- needs array of strings
tests.t2()
--]]

print("Completed tests in test_QDF.lua")

-- return tests
--
-- Following are for some gc experimentation not for QDF
-- tests.t3()
-- tests.t4()
