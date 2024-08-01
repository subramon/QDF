G = {} -- to bypass strict 
G.debug= true
require 'strict'
local lQDF = require 'lQDF'
local tests = {}
-- TODO P2 Need better tests. This one is very basic
--===============================
tests.fill = function()
  local src = lQDF({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, })
  src = src:convert("F4")
  local tim = lQDF({1, 2, 3, 1, 2, 3, 1, 2, 3, 1, })
  tim = tim:convert("I4")
  local grp = lQDF({1, 2, 2, 3, 3, 3, 4, 4, 4, 4,  })
  grp = grp:convert("I4")
  local cnt = grp:srt_grp_cnt()
  assert(cnt:num_elements() == 4)
  assert(cnt:min() == 1)
  assert(cnt:max() == 4)
  local nn = lQDF({0, 1, 0, 1, 0, 1, 1, 0, 1, 0})
  nn = nn:convert("I1")
  src:set_nn(nn)
  assert(src:has_nn())
  local dst = src:fill(cnt, tim) 
  print(src)
  print(nn)
  print(dst)

  print("Test fill completed successfully")
end
tests.fill()
-- return tests
