local function is_in_tbl(x, X)
  assert(type(X) == "table")
  for k, v in ipairs(X) do
    if ( v == x ) then
      return true
    end
  end
  return false
end
return is_in_tbl
