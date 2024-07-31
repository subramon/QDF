-- TODO P1 require 'strict'
local Q      = require 'Q'
local Scalar = require 'libsclr'

local function t1(x_file, x_qtype, y_file, y_qtype)
  local cutils = require 'libcutils'
  local T = Q.load_csv(x_file,  { { name = "x", qtype = x_qtype} })
  local x = T.x
  assert(type(x) == "lVector")
  x:eval()

  local T   = Q.load_csv(y_file, { { name = "y", qtype = y_qtype} })
  local y = T.y
  assert(type(y) == "lVector")
  y:eval()

  assert(x:num_elements()  == y:num_elements())

  --========================================
  local b = Q.is_prev(x, "lt", { default_val = false })
  local r = Q.sum(b)
  local n1, n2 = r:eval()
  if ( n1:to_num() ~= 0 ) then return false end 
  --========================================
  local r = Q.sum(b)
  local n1, n2 = r:eval()
  if ( n1:to_num() ~= 0 ) then return false end 
  --========================================
  local max_x = Q.max(x):eval(); assert(type(max_x) == "Scalar")
  max_x = max_x + Scalar.new(1, x_qtype) -- just bigger than x 

  local max_y = Q.max(y):eval(); assert(type(max_y) == "Scalar")
  max_y = max_y + Scalar.new(1, y_qtype) -- just bigger than y 
  local next_x = Q.vshift(x, 1, max_x)
  local next_y = Q.vshift(y, 1, max_y)
  local t1 = Q.vveq(x, next_x)
  local t2 = Q.vvgeq(y, next_y) -- note the geq
  assert(type(t2) == "lVector")
  local t3 = Q.vvand(t1, t2)
  local r      = Q.sum(t3)
  local n1, n2 = r:eval()
  -- print(n1, n2)
  -- Q.print_csv({x, y, next_x, next_y, t1, t2, t3}, { opfile = "_x" })
  if ( n1:to_num() ~= 0 ) then return false end 
  -- cleanup
  cutils.unlink(x_file)
  cutils.unlink(y_file)
  return true
end
local x_file  = assert(arg[1])
local x_qtype = assert(arg[2])
local y_file  = assert(arg[3])
local y_qtype = assert(arg[4])
local status = t1(x_file, x_qtype, y_file, y_qtype)
print("test status = ", status)
assert(status)
