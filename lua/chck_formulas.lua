local function chck_formulas(F)
  assert(type(F) == "table")
  local nF = 0
  for k, f in pairs(F) do 
    assert(type(f) == "table")
    assert(type(f.components) == "table")
    if ( f.lags ) then 
      assert(type(f.lags) == "table")
      -- This is okay: assert(#f.lags > 0)
      for k1, v1 in ipairs(f.lags) do
        assert(type(v1) == "number")
        assert(v1 > 0)
        for k2, v2 in ipairs(f.lags) do
          if ( k1 ~= k2 ) then
            assert(v1 ~= v2)
          end
        end
      end
    end
    nF = nF + 1
  end
  -- check at least one formula 
  assert(nF >= 1)
  assert(nF <= 16) -- current limitation enforced by plp2_err_bmask
  -- in key value store. Can be relaxed
  -- check that dependence, if defined, is valid
  for k, f in pairs(F) do
    local found = false
    if ( f.dependence ) then
      assert(type(f.dependence) == "string")
      for k2, _ in pairs(F) do
        if ( k2 == f.dependence ) then
          found = true
          break
        end
      end
      assert(found)
    end
  end
  --========================================
  return  true
end
return chck_formulas
