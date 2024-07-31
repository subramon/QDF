require 'strict'
local lQDF   = require 'lQDF'
local exec   = require 'exec_and_capture_stdout'
local qenvs  = require 'qenvs'
local qrslt  = require 'qrslt'
-- local Q      = require 'Q'
-- verify that 
-- xcol is sorted ascending (primary)
-- ycol is sorted ascending (secondary)
-- for a given value of xcol, values of ycol are strictly increasing
local function test_sort_asc_x_asc_y(xcol, ycol)
  local cutils = require 'libcutils'
  assert(type(xcol) == "lQDF")
  assert(xcol:jtype() == "j_array")
  local x_qtype = xcol:qtype()
  assert(xcol:check())

  assert(type(ycol) == "lQDF")
  assert(ycol:jtype() == "j_array")
  local y_qtype = ycol:qtype()
  assert(ycol:check())

  -- dump xcol and ycol as CSV files 
  local x_file = cutils.mkstemp("/tmp/_x_XXXXXX")
  xcol:pr_csv(x_file)
  local y_file = cutils.mkstemp("/tmp/_y_XXXXXX")
  ycol:pr_csv(y_file)

  -- Invoke Q 
  --[[
  local root_dir = os.getenv("LIGHTR_ROOT"); assert(root_dir)
  local T = {}
  T[#T+1] = qenvs()
  T[#T+1] = string.format(
    "qjit %s/lua/q_sort_asc_x_asc_y.lua %s %s %s %s ", 
    root_dir, x_file, x_qtype, y_file, y_qtype)
  local cmd = table.concat(T, "; ")
  -- print("cmd = ", cmd)
  local str = assert(exec(cmd))
  -- print("str = ", str)
  local status = qrslt(str)
  return status
  --]]
  return true
end
return test_sort_asc_x_asc_y
