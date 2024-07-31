local function chk_expected_elem_type(M, elem_type)
  if ( M == nil ) then return end
  assert(type(M) == "table")
  assert(type(elem_type) == "string")
  if ( M and M.elem_type ) then 
    assert(type(M.elem_type) == "string")
    if ( M.elem_type == "F8") then
      assert(elem_type == "number")
    elseif ( M.elem_type == "string") then
      assert(elem_type == "string")
    else
      error(M.elem_type)
    end
  end
end
return chk_expected_elem_type
