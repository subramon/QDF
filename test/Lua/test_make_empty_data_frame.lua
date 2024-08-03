G = {} -- to avoid strict complaints
G.debug= true
local plfile = require 'pl.file'
local plstringx = require 'pl.stringx'
-- above must come before strict 
require 'strict'
local lQDF = require 'lQDF'
local tests = {}
function file_as_str(file)
    local f = assert(io.open(file, "rb"))
    local content = f:read("*all")
    f:close()
    return content
end


tests.make_empty_data_frame = function ()
  local col_names =  { 
    "i1", "i2", "i4", "i8", "f4", "f8", "tm",  }
  local qtypes = { 
    "I1", "I2", "I4", "I8", "F4","F8", "TM1:%Y-%m-%d", } 
  local x = lQDF.make_empty_data_frame(col_names, qtypes, 20)
  assert(type(x) == "lQDF")
  assert(x:check())
  local ncols  = #col_names
  local is_df = x:is_data_frame()
  assert(is_df == true)
  local n = x:obj_arr_len()
  local sz = x:obj_arr_size()
  assert(n == 0)
  assert(sz == 20)
  --===============================
  for k, col in pairs(col_names) do 
    local rbc = x:get(col)
    assert(type(rbc) == "lQDF")
    assert(rbc:jtype() == "j_array")
    if ( rbc:qtype() ~= "TM1" ) then 
      assert(rbc:qtype() == qtypes[k])
    end
    assert(rbc:num_elements() == n)
    assert(rbc:check())
  end
  --===============================
  x:pr_df_as_csv( col_names, "/tmp/foo.csv")
  local str = plfile.read("/tmp/foo.csv")
  str = plstringx.strip(str)
  assert(str == "i1,i2,i4,i8,f4,f8,tm")
  print("Test make_empty_data_frame completed successfully")
end
tests.make_empty_data_frame()
-- return tests
