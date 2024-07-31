G = {} -- to circumvent strict 
G.debug = true 
local plfile = require 'pl.file'
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

tests.place_in_data_frame = function ()
  local cols =  { "i1", "i2", "i4", "i8", "f4", "f8", } 
  local qtypes = { "I1", "I2", "I4", "I8", "F4","F8", }

  local xsz = 20
  local x = lRBC.make_empty_data_frame(cols, qtypes, xsz)
  assert(type(x) == "lRBC")
  assert(x:check())
  assert(x:jtype() == "j_object")
  local is_df = x:is_data_frame()
  assert(is_df == true)
  --===============================
  local optargs = { is_hdr = true }
  local infile = "data_place_in_data_frame_1.csv"
  local y = assert(lRBC.read_csv(cols, qtypes, infile, optargs))
  local is_df = y:is_data_frame()
  local ny = y:obj_arr_len()
  assert(ny == 9)
  for k, v in pairs(cols) do
    local col = y:get(v)
    assert(type(col) == "lRBC")
    assert(col:jtype() == "j_array")
    assert(col:num_elements() == ny)
    assert(col:arr_size() == ny)
  print("XXXXXXXXXXXXXXXXXX")
    x:place_in_data_frame(v, col)

  end
  x:pr_df_as_csv(nil, "_x.csv")
  local is_df, n, sz = x:is_data_frame()
  assert(sz == xsz)
  assert(x:check())
  -- quick check of output 
  local str1 = plfile.read("_x.csv")
  local str2 = plfile.read("out_data_place_in_data_frame_1.csv")
  assert(str1 == str2)
  print("Test place_in_data_frame completed successfully")
end
tests.place_in_data_frame()
-- return tests
