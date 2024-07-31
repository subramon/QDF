-- used to convert SC:n into SC 
local function trim_qtype(x)
  assert(type(x) == "string")
  local j, k = string.find(x, ":")
  if ( j ) then 
    assert(j == k)
    return string.sub(x, 1, j-1)
  else
    return x
  end
end
return trim_qtype
