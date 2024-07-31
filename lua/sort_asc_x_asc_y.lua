-- TODO P1 require 'strict'
local lQDF = require 'lQDF'
-- TODO P3: See if sort is needed at all before sorting 

-- wil sort the dataframe indf so that 
-- xcol is sorted ascending 
-- ycol is sorted ascending 
local function sort_asc_x_asc_y(indf, xcol, ycol)
  -- Note that indf is sorted in place
  -- usually xfld is "co_loc_ref_i"
  -- usually yfld is "t_epoch" derived from "week_start_date"
  -- If is_mod_ycol == true, we apply the permutation  of the sort to ycol
  -- and return a mod_ycol
  if ( G and G.debug ) then 
    assert(type(indf) == "lQDF")
    assert(indf:check())
    assert(indf:is_df() == true)
    local n = indf:obj_arr_len()
    if ( n <= 1 ) then return nil end 
    assert(type(xcol) == "lQDF")
    assert(xcol:check())
    assert(xcol:jtype() == "j_array")

    assert(type(ycol) == "lQDF")
    assert(ycol:check())
    assert(ycol:jtype() == "j_array")
  end
  -- create sort order
  local idx = lQDF.mk_sort_idx_2(xcol, ycol)
  if ( G and G.debug ) then 
    assert(type(idx)   == "lQDF")
    assert(idx:jtype() == "j_array")
    assert(idx:qtype() == "I4")
  end
  -- re-order each column in the indf data frame 
  for i, col in ipairs(indf:keys()) do 
    local srtcol = indf:get(col):permute(idx)
    indf:get(col):set(srtcol)
  end
  if ( is_debug ) then 
    local mod_ycol = ycol:permute(idx)
    assert(mod_ycol:check())
    assert(test_sort_asc_x_asc_y(xcol, mod_ycol))
  end
  end
  return true
end
return sort_asc_x_asc_y
