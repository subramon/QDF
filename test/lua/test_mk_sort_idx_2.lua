G = {} -- to bypass strict 
G.debug= true
local plfile = require 'pl.file'
require 'strict'
local lRBC = require 'lRBC'
local tests = {}

--===============================
local cols =  { "i4", "i8", "f4", "val", "nn_val", }
local qtypes = { "I4", "I8", "F4", "F4", "I1",  }
local optargs = { is_hdr = true }
local infile = "data_mk_sort_idx_2.csv"
local indf = assert(lRBC.read_csv(cols, qtypes, infile, optargs))
--===============================
tests.mk_sort_idx_2 = function ()
  local src1 = indf:get("i4")
  local src2 = indf:get("i8")
  assert(type(src2) == "lRBC")

  local idx = lRBC.mk_sort_idx_2(src1, src2)
  assert(type(idx) == "lRBC")
  assert(idx:jtype() == "j_array")
  assert(idx:qtype() == "I4")
  -- check that sort order is good 
  local permute_src1 = src1:permute(idx)
  assert(permute_src1:qtype() == src1:qtype())
  local permute_src2 = src2:permute(idx)
  assert(permute_src2:qtype() == src2:qtype())

  assert(permute_src1:max() == src1:max())
  assert(permute_src1:min() == src1:min())
  assert(permute_src1:sum() == src1:sum())

  assert(permute_src2:max() == src2:max())
  assert(permute_src2:min() == src2:min())
  assert(permute_src2:sum() == src2:sum())

  local shift_src1 = permute_src1:shift_down(1)
  local shift_src2 = permute_src2:shift_down(1)
  local x = shift_src1:gt(permute_src1)
  local y = shift_src2:gt(permute_src2)
  local z = y:logical_and(x)
  assert(z:sum() == 0)
  print("Test mk_sort_idx_i4_i8 completed successfully")
end
tests.empty_df = function ()
  -- Now testing permute
  -- make an empty data frame
  local cols =  { "i4", "i8", "f4", "val", "nn_val", }
  local qtypes = { "I4", "I8", "F4", "F4", "I1",  }

  local sz = indf:obj_arr_len()
  local outdf = lRBC.make_empty_data_frame(cols, qtypes, sz)
  assert(type(outdf) == "lRBC")
  local idx = lRBC.seq(0, 1, "I4", sz, 0)

  for k, col in ipairs(cols) do
    outdf:get(col):set(indf:get(col):permute(idx))
  end
  -- START DANGEROUS
  for k, col in ipairs(cols) do
    outdf:get(col):set_arr_len(sz)
  end
  outdf:set_obj_arr_len(sz)
  -- STOP  DANGEROUS
  outdf:pr_df_as_csv(nil, "/tmp/_x.csv")
  -- Check contents of output file 
  local status = os.execute("cat /tmp/_x.csv | sed s'/.000000//'g > /tmp/_y.csv")
  local status = os.execute("diff /tmp/_y.csv " .. infile)
  assert(status == 0)
  print("Test empty_df completed successfully")
end
tests.srt_grp_cnt_0 = function()
  local x = lRBC({4, 4, 7, 7, 7, 9, 9, 9, 9, })
  local y = x:convert("I4")
  local z = y:srt_grp_cnt()
  local str = tostring(z)
  assert(str == "[2, 3, 4]")
  print("Test srt_grp_cnt_0 completed successfully")
end
tests.srt_grp_cnt = function()
  -- Now testing srt_grp_cnt
  local grp = indf:get("i4"):sort("ascending"):srt_grp_cnt()
  local str = tostring(grp)
  assert(str == "[4, 4, 4, 4, 4, 4, 4, 4, 4]")
  print("Test srt_grp_cnt completed successfully")
end
tests.fill = function()
  local infill = outdf:get("val")
  infill:set_nn(outdf:get("nn_val"))
  local outfill = infill:fill(grp)
  assert(type(outfill) == "lRBC")
  assert(outfill:jtype() == "j_array")
  assert(outfill:qtype() == "F4")
  print(infill)
  print(outfill)
  print("Test fill completed successfully")
end
tests.mk_sort_idx_2()
tests.empty_df()
tests.srt_grp_cnt()
tests.srt_grp_cnt_0()
-- TODO tests.fill()
-- return tests
