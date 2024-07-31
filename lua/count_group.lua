-- TODO P1 require 'strict'
local lQDF = require 'lQDF'
local function count_group(x)
  -- x is array sorted ascending 
  -- get the number of times each unique value of x occurs
  -- for example, if x has values 4, 4, 7, 7, 7, 9, 9, 9, 9
  -- then n_in_grp = [2,3,4]
  local n_in_grp  = x:srt_grp_cnt()
  if ( G and G.debug ) then 
    assert(type(n_in_grp) == "lQDF")
    assert(n_in_grp:jtype() == "j_array")
    assert(n_in_grp:qtype() == "I4")
  end
  return n_in_grp
end
return count_group
