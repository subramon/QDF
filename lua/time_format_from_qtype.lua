local function time_format_from_qtype(x)
  assert(type(x) == "string")
  local j, k = string.find(x, ":")
  if ( not j ) then return nil end 
  assert(j == k)
  local y = string.sub(x, j+1)
  assert(#y > 0)
  return y
  --[[
  if ( j ) then 
    assert(j == k)
    return string.sub(x, 1, j-1)
  else
    return x
  end
  --]]
end
return time_format_from_qtype
-- local x = "TM1:%Y-%m-%d"
-- local y = time_format_from_qtype(x)
-- print(x, y)
