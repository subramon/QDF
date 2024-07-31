-- TODO P1 require 'strict'
local Q = require 'Q'

local function t1(x_file, x_qtype, n_x_file, n_x_qtype)
  local cutils = require 'libcutils'

  local T   = Q.load_csv(x_file, {{ name = "q_x", qtype = x_qtype}})
  local q_x = T.q_x:eval()
  local T = Q.load_csv(n_x_file, {{ name = "q_n_x", qtype = n_x_qtype}})
  local q_n_x = T.q_n_x:eval()

  local n1, n2 = Q.sum(q_n_x):eval()
  if (n1:to_num() ~= q_x:num_elements()) then return false end 
  -- cleanup
  cutils.unlink(x_file)
  cutils.unlink(n_x_file)
  return true
end

local x_file    = assert(arg[1])
local x_qtype   = assert(arg[2])
local n_x_file  = assert(arg[3])
local n_x_qtype = assert(arg[4])
local status    = t1(x_file, x_qtype, n_x_file, n_x_qtype)
print("test status = ", status)
assert(status)
