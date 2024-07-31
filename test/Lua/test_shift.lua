local lRBC = require 'lRBC'
local simdjson = require 'simdjson'
local tests = {}

tests.shift = function()
  local x, y, z, w
  local str_json = '[ 10, 20, 30, 40, 50, 60]'
  local tbl_json = simdjson.parse(str_json)
  assert(type(tbl_json) == "table")
  x = lRBC(tbl_json)
  assert(type(x) == "lRBC")
  assert(x:qtype() == "F8")

  local nx = x:num_elements()
  --========== shift down
  local y, nn_y = x:shift_down(1)
  local sum1 = nn_y:sum()
  assert(sum1 == (nx-1) )
  assert(nn_y:min() == 0)
  assert(nn_y:max() == 1)

  assert(y:min() == 0)
  assert(y:max() == 50)

  assert(tostring(nn_y) == "[0, 1, 1, 1, 1, 1]")
  assert(tostring(y) == "[0, 10, 20, 30, 40, 50]")
  --========== shift up
  local y, nn_y = x:shift_up(1)
  local sum1 = nn_y:sum()
  assert(sum1 == (nx-1) )
  assert(nn_y:min() == 0)
  assert(nn_y:max() == 1)

  assert(y:min() == 0)
  assert(y:max() == 60)

  assert(tostring(nn_y) == "[1, 1, 1, 1, 1, 0]")
  assert(tostring(y) == "[20, 30, 40, 50, 60, 0]")
 
  print("Test shift completed successfully")
end

tests.shift()
