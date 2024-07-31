local function tbl_elem_type(T)
  assert(type(T) == "table")
  local nT = #T
  --=======================
  local is_uniform = true
  local elem_type = type(T[1])
  for i = 2, nT do
    assert(type(T[i] ~= "nil"))
    assert(type(T[i] ~= "function"))
    if ( elem_type ~= type(T[i]) ) then
      is_uniform = false
      break
    end
  end
  -- elem_type must be number, string, boolean
  -- for it to be considered unfiorm
  -- As an example, if each element of T is a table,
  -- it will be considered non-uniform
  if ( is_uniform ) then
    if ( not ( 
      ( elem_type == "number") or 
      ( elem_type == "string") or 
      ( elem_type == "boolean") ) ) then
      is_uniform = false
    end
  end
  --=======================
  if ( not is_uniform ) then elem_type = nil end 
  return is_uniform, elem_type
end
return tbl_elem_type
