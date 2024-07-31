local function chck_formula_specific_plp(F, P)
if ( P ) then
  assert(type(P) == "table")
  local nF = 0
  for k, v in pairs(F) do
    nF = nF + 1
  end
  local nP = 0
  for k, v in pairs(P) do
    nP = nP + 1
    if ( not v ) then 
      assert(type(v) == "function")
    end
  end
  assert(nF >= nP)
  return true
end
end
return chck_formula_specific_plp
