local ffi = require 'ffi'
local function is_in_meta(v2, M)
  local found = false
  for k3, v3 in ipairs(M) do
    if ( v2 == v3.name ) then
      found = true; break;
     end
 end
  return found 
end

local function chck_components(C, M)
  assert(type(M) == "table") -- out_meta 
  assert(type(C) == "table")
  local nC = 0
  -- following because we use an 8 byte bit mask to indicate
  -- which components are to be used 
  assert(#C <= 8 * ffi.sizeof("uint64_t"))
  for k, v in pairs(C) do 
    assert(type(k) == "string")
    assert(type(v) == "table")

    -- START: Check depends_on
    assert(type(v.depends_on) == "table")
    assert(#v.depends_on > 0)

    for k2, v2 in ipairs(v.depends_on) do 
      assert(type(v2) == "string")
      for k3, v3 in ipairs(v.depends_on) do 
        if ( k2 ~= k3 ) then assert(v2 ~= v3) end 
      end
    local found = is_in_meta(v2, M)
      assert(found == true, 
       "Column " .. v2 .. " not found in meta data for R dataframe")
    end
    -- STOP: Check depends_on

    assert(type(v.class) == "number")
    assert((v.class >= 1 ) and ( v.class <= 6))
    if ( v.class == 1 ) then 
      assert(#v.depends_on == 1)
    elseif ( v.class == 2 ) then 
      assert(#v.depends_on == 1)
    elseif ( v.class == 3 ) then 
      if (#v.depends_on == 1) then
        assert(v.by == nil)
      elseif (#v.depends_on == 2) then
        local by = assert(v.by)
        assert(type(by) == "string")
        assert(is_in_meta(by, M))
      else
        error("model class 3 cannot depend on > 2 columns")
      end
    elseif ( v.class == 4 ) then 
      assert(v.factor_name ~= v.feature_name)
      assert(#v.depends_on == 2)
      assert(is_in_meta(v.feature_name, M))
      assert(is_in_meta(v.factor_name,  M))
    elseif ( v.class == 5 ) then 
      assert(#v.depends_on == 2)
      assert(is_in_meta(v.feature_x, M))
      assert(is_in_meta(v.feature_y,  M))
      assert(v.feature_x ~= v.feature_y)
    elseif ( v.class == 6 ) then 
      assert(#v.depends_on == 1)
    end 

    assert(type(v.Rstr) == "string")
    assert(#v.Rstr > 0)

    nC = nC + 1
  end
  assert(nC >= 1)
  for k1, v1 in pairs(C) do 
    for k2, v2 in pairs(C) do 
      if ( k1 ~= k2 ) then
        assert(v1.Rstr ~= v2.Rstr)
      end
    end
  end
  return  true
end
return chck_components
