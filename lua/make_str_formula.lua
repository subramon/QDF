local is_in_tbl = require 'is_in_tbl'
local function make_str_formula(C, dropC, nC, component_to_idx,
  is_debug, model_over_rides)
  assert(type(C) == "table")
  assert(type(dropC) == "table")
  if ( model_over_rides ) then 
    assert(type(model_over_rides) == "table")
  end
  assert(type(is_debug) == "boolean")
  assert(type(component_to_idx) == "table")
  --==============================================
  if ( is_debug ) then 
    for _, col in ipairs(dropC) do 
      local is_found = false
      for k, c in pairs(C) do 
        if ( c.col == col ) then is_found = true; break end 
      end
      assert(is_found == true)
    end
  end
  --==============================================
  local T = {}
  T[#T+1] = "sls_unit_q ~ "
  local first = true
  for k, c in pairs(C) do  -- for each component in C 
    if ( is_in_tbl(c.col, dropC) ) then
      -- print("Skipping component " .. c.col) 
    else
      -- print("Adding  component " .. c.col) 
      if ( first ) then 
        first = false
      else 
        T[#T+1] = " + "; 
      end 
      local Rstr = c.Rstr
      if ( ( model_over_rides ) and ( model_over_rides[c.col] ) ) then 
        assert(type(model_over_rides[c.col]) == "table")
        Rstr = assert(model_over_rides[c.col].Rstr)
      end
      assert(type(Rstr) == "string")
      T[#T+1] = Rstr
    end
  end
  local formula_str = string.gsub(table.concat(T, " "), "\n", " ")
  --========================================================
  -- nC = #G.c_components
  -- inform C which components will be used 
  local U = {}
  for k = 1, nC do U[k] = "1" end -- assume all will be used

  for k, v in pairs(dropC) do 
    local idx = component_to_idx[v]
    -- print("idx for name = ", idx, v)
    U[idx] = "0"
  end
  local str_is_component_used = table.concat(U, "")
  -- print("str_is_component_used = ", str_is_component_used)
  --========================================================
  return formula_str, str_is_component_used
end
return make_str_formula
