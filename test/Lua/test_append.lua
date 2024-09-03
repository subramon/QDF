G = {} -- to avoid problems with strict
G.debug= true
require 'strict'
local lQDF = require 'lQDF'
local tests = {}

tests.append = function ()
  local col_names =  { "i1", "i2", "i4", "i8", "f4", "f8", "tm", }
  local qtypes = { "I1", "I2", "I4", "I8", "F4","F8", "TM1:%Y-%m-%d", } 
  local ncols  = #col_names
  --===============================
  local infile = "../data/append_src.csv"
  local optargs = { is_hdr = true }
  local src = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))
  assert(type(src) == "lQDF")
  local is_df = src:is_data_frame(); assert(is_df == true)
  local sn    = src:obj_arr_len(); assert(sn == 7)
  --===============================
  local infile = "../data/append_dst.csv"
  local optargs = { is_hdr = true }
  local dst = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))
  local is_df = lQDF.is_data_frame(dst); assert(is_df == true)
  local dn    = dst:obj_arr_len(); assert(dn == 4)
  --===============================
  local x = dst:append_df(src)
  -- since there is not enough space in dst, append_df returns a number
  assert(type(x) == "number") 
  assert(x == 7)
  -- dst has been unchanged 
  local is_df = dst:is_df(); assert(is_df == true)
  local dn    = dst:obj_arr_len(); assert(dn == 4)
  --===============================
  local infile = "../data/append_dst.csv"
  local optargs = { is_hdr = true, buf_spec = { absolute = 11 } }
  local dst = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))
  local is_df = dst:is_data_frame(); assert(is_df == true)
  local dn    = dst:obj_arr_len(); assert(dn == 4)
  local tmp = dst:get("i1")
  assert(type(tmp) == "lQDF")
  assert(tmp:jtype() == "j_array")
  local ds = tmp:arr_size()
  assert(ds == 11)
  local dn = tmp:num_elements()
  assert(dn == 4)
  --===============================
  local ret = dst:append_df(src, true)
  assert(type(ret) == "lQDF")
  --===============================
  for k, col in pairs(col_names) do 
    local rbc = dst:get(col)
    assert(type(rbc) == "lQDF")
    assert(rbc:jtype() == "j_array")
    if ( rbc:qtype() ~= "TM1" ) then 
      assert(rbc:qtype() == qtypes[k])
    end
    assert(rbc:num_elements() == sn+dn)
    assert(rbc:check())
  end
  assert(src:check())
  assert(dst:check())

  --===============================
  dst:pr_df_as_csv( col_names, "/tmp/foo.csv")
  print("Test append completed successfully")
end
tests.append_empty = function ()
  local cols =  { "i1", "i2", "i4", "i8", "f4", "f8", "tm",  }
  local qtypes = { "I1", "I2", "I4", "I8", "F4","F8", "TM1:%Y-%m-%d", } 
  --===============================
  local infile = "../data/append_src.csv"
  local optargs = { is_hdr = true }
  local src1 = assert(lQDF.read_csv(cols, qtypes, infile, optargs))
  local is_df = src1:is_data_frame(); assert(is_df == true)
  local s1n = src1:obj_arr_len()
  --===============================
  local infile = "../data/append_dst.csv"
  local src2 = assert(lQDF.read_csv(cols, qtypes, infile, optargs))
  local is_df = src2:is_data_frame(); assert(is_df == true)
  local s2n = src2:obj_arr_len()
  --===============================
  local dst = lQDF.make_empty_data_frame(cols, qtypes, s1n+s2n)
  assert(type(dst) == "lQDF")
  local ret = dst:append_df(src1)
  print("XXX", type(ret));
  assert(type(ret) == "lQDF")
  local ret = dst:append_df(src2)
  assert(type(ret) == "lQDF")
  --===============================
  for k, col in pairs(cols) do 
    local rbc = dst:get(col)
    assert(type(rbc) == "lQDF")
    assert(rbc:jtype() == "j_array")
    if ( rbc:qtype() ~= "TM1" ) then 
      assert(rbc:qtype() == qtypes[k])
    end
    assert(rbc:num_elements() == s1n+s2n)
    assert(rbc:check())
  end
  assert(dst:check())
  --===============================
  dst:pr_df_as_csv( cols, "/tmp/bar.csv")
  print("Test append_empty completed successfully")
end
tests.resize_1 = function ()
  local col_names =  { "i1", "i2", "i4", "i8", "f4", "f8", "tm", }
  local qtypes = { "I1", "I2", "I4", "I8", "F4","F8", "TM1:%Y-%m-%d", } 
  local ncols  = #col_names
  --===============================
  local infile = "../data/append_src.csv"
  local optargs = { is_hdr = true }
  local src = assert(lQDF.read_csv(col_names, qtypes, infile, optargs))
  assert(type(src) == "lQDF")
  local is_df  = src:is_data_frame(); assert(is_df == true)
  local src_n  = src:obj_arr_len();   assert(src_n == 7)
  local src_sz = src:obj_arr_size();  assert(src_sz == 7)
  --===============================
  local dst = src:resize_df(14)
  local is_df  = dst:is_data_frame(); assert(is_df == true)
  local dst_n  = dst:obj_arr_len();   assert(dst_n == 7)
  local dst_sz = dst:obj_arr_size();  assert(dst_sz == 14)
  dst:pr_df_as_csv(nil, "_resize.csv")
  -- TODO Compare _resize.csv with chk_resize.csv
  --===============================

  print("Test resize_1 completed successfully")
end

tests.append()
tests.append_empty()
tests.resize_1()
-- return tests
