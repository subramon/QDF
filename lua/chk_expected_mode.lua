local function chk_expected_mode(M, is_uniform)
  if ( M == nil ) then return end
  assert(type(M) == "table")
  assert(type(is_uniform) == "boolean")
  if ( M and M.mode ) then 
    if ( M.mode == "uniform") then
      assert(is_uniform == true)
    elseif ( M.mode == "mixed" ) then
      -- okay
    else
      error("Bad mode " .. M.mode)
    end
  end
end
return chk_expected_mode
