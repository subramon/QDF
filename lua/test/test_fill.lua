-- Because of strange problem with invoking Lua packages from JNI
-- I had to get rid of plfile
-- local plfile = require 'pl.file'
require 'strict'
local ffi = require 'ffi'
local lQDF = require 'lQDF'
local cast_ptr = require 'cast_ptr'
local file_to_str = require 'file_to_str'
local exec = require 'exec_and_capture_stdout'
local clib -- in closure of test_fill

local function load_so(
  )
  local root_dir = assert(os.getenv("LIGHTR_ROOT"))
  local src = root_dir .. "/lua/test_fill.c"
  local inc = root_dir .. "/lua/test_fill.h"
  local so  = root_dir .. "/src/libfill.so"
  local incs = "-I" .. root_dir 
  local cmd = string.format("gcc -g -shared -fPIC %s %s -o %s ",
    src, incs, so)
  -- print("Compiling and loading: ", cmd)
  local rslt = exec(cmd)
  assert(rslt)
  -- ffi.cdef(plfile.read(inc)) -- see reason for commenting out above
  ffi.cdef(file_to_str(inc))
  clib = ffi.load("libfill.so")
  assert(clib.test_fill)

end

-- verify that fill is done correctly
-- This code only tests back fill and forward fill
-- TODO P2 Need to test averaging as well
local function test_fill(pre, nn_pre, post, grp, n_in_grp, tim)
  local status = 0

  assert(type(pre) == "lQDF")
  assert(pre:qtype() == "F4")
  assert(pre:jtype() == "j_array")
  assert(pre:check())
  local n_pre, c_pre    = pre:get_arr_ptr()
  c_pre = ffi.cast("const float * const ", c_pre)

  assert(type(nn_pre) == "lQDF")
  assert(nn_pre:qtype() == "I1")
  assert(nn_pre:jtype() == "j_array")
  assert(nn_pre:check())
  local n_nn_pre, c_nn_pre = nn_pre:get_arr_ptr()
  c_nn_pre = ffi.cast("const int8_t * const ", c_nn_pre)

  assert(type(post) == "lQDF")
  assert(post:qtype() == "F4")
  assert(post:jtype() == "j_array")
  assert(post:check())
  local n_post, c_post   = post:get_arr_ptr()
  c_post = ffi.cast("const float * const ", c_post)

  assert(type(grp) == "lQDF")
  assert(grp:qtype() == "I2")
  assert(grp:jtype() == "j_array")
  assert(grp:check())
  local n_grp, c_grp   = grp:get_arr_ptr()
  c_grp = cast_ptr(c_grp, grp:qtype())
  c_grp = ffi.cast("const int16_t * const ", c_grp)

  assert(type(tim) == "lQDF")
  assert(tim:qtype() == "I4")
  assert(tim:jtype() == "j_array")
  assert(tim:check())
  local n_tim, c_tim   = tim:get_arr_ptr()
  c_tim = cast_ptr(c_tim, tim:qtype())
  c_tim = ffi.cast("const int32_t * const ", c_tim)

  assert(type(n_in_grp) == "lQDF")
  assert(n_in_grp:qtype() == "I4")
  assert(n_in_grp:jtype() == "j_array")
  assert(n_in_grp:check())

  assert(n_pre == n_nn_pre)
  assert(n_pre == n_nn_pre)
  assert(n_pre == n_post)
  assert(n_pre == n_grp)

  if ( not clib ) then load_so() end 
  --[[
    pre:pr_csv("_pre.csv")
    post:pr_csv("_post.csv")
    nn_pre:pr_csv("_nn_pre.csv")
    grp:pr_csv("_grp.csv")
    n_in_grp:pr_csv("_n_in_grp.csv")
    --]]
  local junk = 1234.5678
  local status = clib.test_fill(junk, c_pre, c_nn_pre, c_post, c_grp, n_pre)
  if ( status == 0 ) then return true else return false end 
  return true
end
return test_fill
-- load_so() 
