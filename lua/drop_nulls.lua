local lQDF = require 'lQDF'
-- indf is a dataframe
-- key is a string
-- we find the column in indf with name key. Call this x 
-- we modify the indf *in place* by squeezing out the rows where
-- x has null values
--
local function drop_nulls(indf, key)
  local is_debug = false 
  if ( G and G.debug == true ) then  is_debug = true end 
  if ( is_debug ) then 
    assert(type(indf) == "lQDF")
    assert(indf:check())
    assert(type(key) == "string")
    assert(indf:is_key(key))
  end
  -- if column identified by key does not have a corresponding nn column
  -- then it cannot have null values. In that case, return early
  local nn_key = "nn_" .. key
  if ( indf:is_key(nn_key) == false ) then 
    return indf:get(key):num_elements()
  end 
  -- If all values of nn column are 1, then no null values => return early
  local sum          = indf:get(nn_key):sum()
  local num_elements = indf:get(nn_key):num_elements()
  if ( is_debug ) then 
    assert(type(sum) == "number")
    assert(type(num_elements) == "number")
    assert(sum >= 0)
    assert(sum <= num_elements)
  end 
  if ( sum == 0 ) then return 0 end -- no good 
  if ( sum == num_elements ) then return num_elements end  -- all good 
  --===== Make "cond": a copy of the nn column 
  local cond = indf:get(nn_key):clone()
  --- Now squeeze out null values base on cond 
  if ( is_debug ) then 
    assert(cond:max() <= 1)
    assert(cond:min() >= 0)
    assert(cond:sum() == sum)
  end
  local n_good = indf:squeeze_where(cond)
  if ( is_debug ) then 
    assert(type(n_good) == "number")
    assert(n_good == sum)
  end
  return n_good
end
return drop_nulls
