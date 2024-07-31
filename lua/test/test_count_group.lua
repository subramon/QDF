require 'strict'
local lQDF = require 'lQDF'
local qenvs = require 'qenvs'
local qrslt = require 'qrslt'
local exec   = require 'exec_and_capture_stdout'

local function test_count_group(x, n_x)
  local cutils = require 'libcutils'
  -- tests count_group()
  assert(type(n_x) == "lQDF") 
  assert(n_x:jtype() == "j_array")
  local n_x_qtype = n_x:qtype()
  assert(n_x_qtype == "I4")
  assert(n_x:check())

  assert(type(x) == "lQDF") 
  assert(x:jtype() == "j_array")
  local x_qtype = x:qtype()
  assert(x_qtype == "I1" or x_qtype == "I2" or x_qtype == "I4")
  assert(x:check())
  -- sum(n_x) == |x|
  
  -- DUMP QDF as CSV for Q 
  local x_file = cutils.mkstemp("/tmp/_x_XXXXXX")
  x:pr_csv(x_file)
  local n_x_file = cutils.mkstemp("/tmp/_n_x_XXXXXX")
  n_x:pr_csv(n_x_file)
  -- Invoke Q
  --[[
  local root_dir = os.getenv("LIGHTR_ROOT"); assert(root_dir)
  local T = {}
  T[#T+1] = qenvs()
  T[#T+1] = string.format(
    "qjit %s/lua/q_count_group.lua %s %s %s %s ", 
    root_dir, x_file, x_qtype, n_x_file, n_x_qtype)
  local cmd = table.concat(T, "; ")
  -- print("cmd = ", cmd)
  local str = assert(exec(cmd))
  local status = qrslt(str)
  return status
  --]]
  return true -- TODO P1 

end
return test_count_group
